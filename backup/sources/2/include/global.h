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
//		0	1	2	3
//	enum { FLOOR, DOOR, LOOT, ENEMY }; 

// MAX_X,MAX_Y are the GAME MAP outer coordinates 
// while X_SCR,Y_SCR are max values (MAX_X/Y+10) for screen (to avoid bugs)
#define MAX_X 80
#define MAX_Y 40

#define X_SCR 80+10
#define Y_SCR 40+10

// Own headers
#include "structs.h"
#include "functions.h"

#endif // TO END THE DEFINITION FUNCTION TO ONE TIME USE ONLY!
