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
#include <ncurses.h>

// Define global types:
//		0	1	2	3	4	5	6
	enum { UNKNOWN, GROUND, LOOT, DOOR, DSTAIRS, USTAIRS, ENEMY} TileType; 
	enum { NO, YES} Ask; 
	enum { RANDOM, FOLLOW} MovMode; 
	enum { DEAD, SPIDER, GOBLIN, TROLL} MonType; 
	enum { OTHER, PLAYER, MONSTER} Actuator; 

///// GLOBAL VARIABLES: ///////////////////////////////
// MAX_X,MAX_Y are the GAME MAP outer coordinates 
// while X_SCR,Y_SCR are max values (MAX_X/Y+10) for screen (to avoid bugs)
int MAX_X; int MAX_Y; int X_SCR; int Y_SCR; int LVLS; int ROOMS;

///////////////////////////////////////////////////////

// Own headers
#include "structs.h"
#include "functions.h"

#endif // TO END THE DEFINITION FUNCTION TO ONE TIME USE ONLY!
