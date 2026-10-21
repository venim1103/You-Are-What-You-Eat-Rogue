#ifndef WEB_CURSES_H
#define WEB_CURSES_H

#include <stdio.h>
#include <stdarg.h>

typedef unsigned int chtype;
#define A_CHARTEXT 0x0000ffff

int initscr(void);
int noecho(void);
int endwin(void);
int clear(void);
int clrtoeol(void);
int refresh(void);
int move(int y, int x);
int mvprintw(int y, int x, const char *fmt, ...);
int printw(const char *fmt, ...);
chtype mvinch(int y, int x);
int getch(void);

#endif
