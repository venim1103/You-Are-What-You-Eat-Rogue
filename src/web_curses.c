#ifdef __EMSCRIPTEN__

#include "web_curses.h"
#include <emscripten.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define WEB_SCR_ROWS 60
#define WEB_SCR_COLS 175

static char screen_buf[WEB_SCR_ROWS][WEB_SCR_COLS];
static int cur_y = 0;
static int cur_x = 0;
static char render_flat_buf[WEB_SCR_ROWS * (WEB_SCR_COLS + 1) + 1];

// Bridge to JavaScript web UI
EM_JS(void, js_init_screen, (), {
    if (window.initRogueScreen) {
        window.initRogueScreen();
    }
});

EM_JS(void, js_update_display, (const char* str, int py, int px), {
    try {
        var text = "";
        if (typeof UTF8ToString !== 'undefined') {
            text = UTF8ToString(str);
        } else if (typeof Module !== 'undefined' && Module.UTF8ToString) {
            text = Module.UTF8ToString(str);
        } else if (typeof stringToUTF8 !== 'undefined' || typeof HEAPU8 !== 'undefined') {
            var len = 0;
            while (HEAPU8[str + len] !== 0) len++;
            var bytes = new Uint8Array(HEAPU8.buffer, str, len);
            text = new TextDecoder('utf-8').decode(bytes);
        }
        if (window.updateRogueDisplay) {
            window.updateRogueDisplay(text, py, px);
        }
    } catch(err) {
        console.error("js_update_display error:", err);
        if (window.showRogueError) {
            window.showRogueError("Display render error: " + err);
        }
    }
});

EM_JS(int, js_getch_async, (), {
    return Asyncify.handleSleep(function(wakeUp) {
        if (window.rogueKeyQueue && window.rogueKeyQueue.length > 0) {
            wakeUp(window.rogueKeyQueue.shift());
            return;
        }
        window.rogueResolveKey = wakeUp;
    });
});

int initscr(void)
{
    clear();
    js_init_screen();
    return 0;
}

int noecho(void)
{
    return 0;
}

int endwin(void)
{
    return 0;
}

int clear(void)
{
    for (int r = 0; r < WEB_SCR_ROWS; r++) {
        for (int c = 0; c < WEB_SCR_COLS; c++) {
            screen_buf[r][c] = ' ';
        }
    }
    cur_y = 0;
    cur_x = 0;
    return 0;
}

int clrtoeol(void)
{
    if (cur_y >= 0 && cur_y < WEB_SCR_ROWS) {
        for (int c = cur_x; c < WEB_SCR_COLS; c++) {
            if (c >= 0) {
                screen_buf[cur_y][c] = ' ';
            }
        }
    }
    return 0;
}

int move(int y, int x)
{
    if (y >= 0 && y < WEB_SCR_ROWS) cur_y = y;
    if (x >= 0 && x < WEB_SCR_COLS) cur_x = x;
    return 0;
}

static void put_string_at(int y, int x, const char *str)
{
    int r = y;
    int c = x;
    while (*str) {
        if (*str == '\n') {
            r++;
            c = 0;
        } else if (*str == '\r') {
            c = 0;
        } else if (*str == '\t') {
            c += 4;
        } else {
            if (r >= 0 && r < WEB_SCR_ROWS && c >= 0 && c < WEB_SCR_COLS) {
                screen_buf[r][c] = *str;
            }
            c++;
        }
        str++;
    }
    cur_y = r;
    cur_x = c;
}

int mvprintw(int y, int x, const char *fmt, ...)
{
    char buf[4096];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    put_string_at(y, x, buf);
    return 0;
}

int printw(const char *fmt, ...)
{
    char buf[4096];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    put_string_at(cur_y, cur_x, buf);
    return 0;
}

chtype mvinch(int y, int x)
{
    if (y >= 0 && y < WEB_SCR_ROWS && x >= 0 && x < WEB_SCR_COLS) {
        return (chtype)(unsigned char)screen_buf[y][x];
    }
    return (chtype)' ';
}

int refresh(void)
{
    int ptr = 0;
    int last_row = 0;

    for (int r = WEB_SCR_ROWS - 1; r >= 0; r--) {
        for (int c = 0; c < WEB_SCR_COLS; c++) {
            if (screen_buf[r][c] != ' ') {
                last_row = r;
                break;
            }
        }
        if (last_row > 0) break;
    }
    if (last_row < 46) last_row = 46; // ensure status line is visible

    for (int r = 0; r <= last_row; r++) {
        int cols_to_print = 160;
        for (int c = WEB_SCR_COLS - 1; c >= 160; c--) {
            if (screen_buf[r][c] != ' ') {
                cols_to_print = c + 1;
                break;
            }
        }
        for (int c = 0; c < cols_to_print; c++) {
            render_flat_buf[ptr++] = screen_buf[r][c];
        }
        render_flat_buf[ptr++] = '\n';
    }
    render_flat_buf[ptr] = '\0';

    js_update_display(render_flat_buf, cur_y, cur_x);
    return 0;
}

int getch(void)
{
    refresh();
    return js_getch_async();
}

#else
// Prevent ISO C empty file warning when compiled natively
typedef int web_curses_dummy_t;
#endif
