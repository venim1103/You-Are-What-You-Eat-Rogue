#ifdef __EMSCRIPTEN__

#include "web_curses.h"
#include <emscripten.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define WEB_SCR_ROWS 60
#define WEB_SCR_COLS 175

static char screen_buf[WEB_SCR_ROWS][WEB_SCR_COLS];
static unsigned char cell_flag_buf[WEB_SCR_ROWS][WEB_SCR_COLS];
static int cur_y = 0;
static int cur_x = 0;
static char render_flat_buf[WEB_SCR_ROWS * (WEB_SCR_COLS + 1) + 1];
static char flag_flat_buf[WEB_SCR_ROWS * (WEB_SCR_COLS + 1) + 1];

// Bridge to JavaScript web UI
EM_JS(void, js_init_screen, (), {
    if (window.initRogueScreen) {
        window.initRogueScreen();
    }
});

EM_JS(void, js_update_display, (const char* str, const char* flags_str, int py, int px), {
    try {
        var text = "";
        var flags = "";
        if (typeof UTF8ToString !== 'undefined') {
            text = UTF8ToString(str);
            flags = UTF8ToString(flags_str);
        } else if (typeof Module !== 'undefined' && Module.UTF8ToString) {
            text = Module.UTF8ToString(str);
            flags = Module.UTF8ToString(flags_str);
        } else if (typeof stringToUTF8 !== 'undefined' || typeof HEAPU8 !== 'undefined') {
            var len = 0;
            while (HEAPU8[str + len] !== 0) len++;
            var bytes = new Uint8Array(HEAPU8.buffer, str, len);
            text = new TextDecoder('utf-8').decode(bytes);
            var flen = 0;
            while (HEAPU8[flags_str + flen] !== 0) flen++;
            var fbytes = new Uint8Array(HEAPU8.buffer, flags_str, flen);
            flags = new TextDecoder('utf-8').decode(fbytes);
        }
        if (window.updateRogueDisplay) {
            window.updateRogueDisplay(text, py, px, flags);
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

EM_JS(void, js_record_run, (int victory, const char* form_name, int form_sym, const char* killer, int xp, int rank, int maxlvl, int turns, int hp, int maxhp, int att, int def), {
    try {
        var readStr = function(ptr) {
            if (!ptr) return "";
            if (typeof UTF8ToString !== 'undefined') return UTF8ToString(ptr);
            if (typeof Module !== 'undefined' && Module.UTF8ToString) return Module.UTF8ToString(ptr);
            if (typeof HEAPU8 !== 'undefined') {
                var len = 0;
                while (HEAPU8[ptr + len] !== 0) len++;
                return new TextDecoder('utf-8').decode(new Uint8Array(HEAPU8.buffer, ptr, len));
            }
            return "";
        };
        if (window.recordRogueRun) {
            window.recordRogueRun({
                victory: !!victory,
                form: readStr(form_name) || "Human",
                symbol: String.fromCharCode(form_sym || 64),
                killer: readStr(killer) || "Unknown",
                xp: xp,
                rank: rank,
                maxlvl: maxlvl,
                turns: turns,
                hp: hp,
                maxhp: maxhp,
                att: att,
                def: def
            });
        }
    } catch (e) {
        console.error("js_record_run error:", e);
    }
});

void web_record_run(int victory, const char *form_name, char form_sym, const char *killer, int xp, int rank, int maxlvl, int turns, int hp, int maxhp, int att, int def)
{
    js_record_run(victory, form_name ? form_name : "Human", (int)(unsigned char)form_sym, killer ? killer : "Unknown", xp, rank, maxlvl, turns, hp, maxhp, att, def);
}

EM_JS(void, js_play_sfx, (const char* sfx_ptr), {
    try {
        if (!sfx_ptr || !window.playRogueSfx) return;
        var sfx = "";
        if (typeof UTF8ToString !== 'undefined') {
            sfx = UTF8ToString(sfx_ptr);
        } else if (typeof Module !== 'undefined' && Module.UTF8ToString) {
            sfx = Module.UTF8ToString(sfx_ptr);
        } else if (typeof HEAPU8 !== 'undefined') {
            var len = 0;
            while (HEAPU8[sfx_ptr + len] !== 0) len++;
            sfx = new TextDecoder('utf-8').decode(new Uint8Array(HEAPU8.buffer, sfx_ptr, len));
        }
        if (sfx) {
            window.playRogueSfx(sfx);
        }
    } catch (e) {
        console.error("js_play_sfx error:", e);
    }
});

void web_play_sfx(const char *sfx)
{
    if (sfx != NULL) {
        js_play_sfx(sfx);
    }
}

EM_JS(unsigned int, js_get_seed_value, (), {
    if (typeof window.getRogueSeedValue === 'function') {
        return (window.getRogueSeedValue() >>> 0);
    }
    return 0;
});

EM_JS(int, js_get_seed_mode, (), {
    if (typeof window.getRogueSeedMode === 'function') {
        return window.getRogueSeedMode() | 0;
    }
    return 0;
});

EM_JS(void, js_get_seed_label, (char* out_buf, int max_len), {
    try {
        var lbl = (typeof window.getRogueSeedLabel === 'function') ? String(window.getRogueSeedLabel() || "") : "";
        if (!out_buf || max_len <= 0) return;
        if (typeof stringToUTF8 === 'function') {
            stringToUTF8(lbl, out_buf, max_len);
        } else if (typeof Module !== 'undefined' && Module.stringToUTF8) {
            Module.stringToUTF8(lbl, out_buf, max_len);
        } else if (typeof HEAPU8 !== 'undefined') {
            var bytes = new TextEncoder().encode(lbl);
            var n = Math.min(bytes.length, max_len - 1);
            for (var i = 0; i < n; i++) {
                HEAPU8[out_buf + i] = bytes[i];
            }
            HEAPU8[out_buf + n] = 0;
        }
    } catch (e) {}
});

unsigned int web_get_initial_seed(int *out_mode, char *out_label, int label_max)
{
    if (out_mode) {
        *out_mode = js_get_seed_mode();
    }
    if (out_label && label_max > 0) {
        out_label[0] = '\0';
        js_get_seed_label(out_label, label_max);
    }
    return js_get_seed_value();
}

EM_JS(void, js_unlock_bestiary, (int sym, int event_type), {
    try {
        if (window.unlockRogueBestiary) {
            window.unlockRogueBestiary(String.fromCharCode(sym || 64), event_type | 0);
        }
    } catch (e) {
        console.error("js_unlock_bestiary error:", e);
    }
});

void web_unlock_bestiary(char sym, int event_type)
{
    js_unlock_bestiary((int)(unsigned char)sym, event_type);
}

EM_JS(int, js_get_bestiary_stat, (int sym, int stat_type), {
    try {
        if (window.getRogueBestiaryStat) {
            return window.getRogueBestiaryStat(String.fromCharCode(sym || 64), stat_type | 0) | 0;
        }
    } catch (e) {}
    return 0;
});

int web_get_bestiary_stat(char sym, int stat_type)
{
    return js_get_bestiary_stat((int)(unsigned char)sym, stat_type);
}

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
            cell_flag_buf[r][c] = 0;
        }
    }
    cur_y = 0;
    cur_x = 0;
    return 0;
}

void web_clear_cell_flags(void)
{
    for (int r = 0; r < WEB_SCR_ROWS; r++) {
        for (int c = 0; c < WEB_SCR_COLS; c++) {
            cell_flag_buf[r][c] = 0;
        }
    }
}

void web_set_cell_flag(int y, int x, unsigned char flag)
{
    if (y >= 0 && y < WEB_SCR_ROWS && x >= 0 && x < WEB_SCR_COLS) {
        cell_flag_buf[y][x] = flag;
    }
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
    static char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    put_string_at(y, x, buf);
    return 0;
}

int printw(const char *fmt, ...)
{
    static char buf[1024];
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
            render_flat_buf[ptr] = screen_buf[r][c];
            flag_flat_buf[ptr] = (char)('0' + (cell_flag_buf[r][c] & 0x07));
            ptr++;
        }
        render_flat_buf[ptr] = '\n';
        flag_flat_buf[ptr] = '\n';
        ptr++;
    }
    render_flat_buf[ptr] = '\0';
    flag_flat_buf[ptr] = '\0';

    js_update_display(render_flat_buf, flag_flat_buf, cur_y, cur_x);
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
