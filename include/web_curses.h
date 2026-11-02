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
void web_clear_cell_flags(void);
void web_set_cell_flag(int y, int x, unsigned char flag);
void web_record_run(int victory, const char *form_name, char form_sym, const char *killer, int xp, int rank, int maxlvl, int turns, int hp, int maxhp, int att, int def);
void web_play_sfx(const char *sfx);
unsigned int web_get_initial_seed(int *out_mode, char *out_label, int label_max);
void web_unlock_bestiary(char sym, int event_type);
int web_get_bestiary_stat(char sym, int stat_type);

#endif
