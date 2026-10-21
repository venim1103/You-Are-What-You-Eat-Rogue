// TO AVOID REUSING THIS CODE TO MAKE NEW VARIABLES EACH TIME:
#ifndef GLOBAL_H
#define GLOBAL_H

// Headers
/*	char joku[12];
	sprintf(joku, "%d", ran);
	mvprintw(0,0,joku);
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __EMSCRIPTEN__
#include "web_curses.h"
#else
#include <ncurses.h>
#endif

// Define global types:
//		0	1	2	3	4	5	6
	enum { UP, DOWN, LEFT, RIGHT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT }; 
	enum { UNKNOWN, GROUND, LOOT, DOOR, DSTAIRS, USTAIRS, ENEMY }; 
	enum { DEAD, SPIDER, SNAKE, GOBLIN, TROLL, GHOST, SLIME, BAT }; 
	enum { FOLLOW, RANDOM_S, RANDOM_X }; 
	enum { OTHER, PLAYER, MONSTER }; 
	enum { NO, YES }; 

///// GLOBAL VARIABLES: ///////////////////////////////
// MAX_X,MAX_Y are the GAME MAP outer coordinates 
// while X_SCR,Y_SCR are max values (MAX_X/Y+10) for screen (to avoid bugs)
extern int MAX_X; extern int MAX_Y; extern int X_SCR; extern int Y_SCR; extern int LVLS; extern int ROOMS;

///////////////////////////////////////////////////////

// Own headers
#include "structs.h"
#include "functions.h"

#endif // TO END THE DEFINITION FUNCTION TO ONE TIME USE ONLY!
