#include "global.h"

// Functions
int main(void) 
{

// CREATE THE LEVEL POINTER
	Level *level=NULL;

// Seed the random
	clock_t ct; ct = clock(); double clk = ct; srand(clk);
// Init
	ScreenInit();

// MAIN LOOP
	level = Game(level);

//	PrintNum(level->mons,"MONS:  .",8,6);
// Exit
	// level[0].maxlvl is the last created lvl id number

	FreeAll(level, level->maxlvl);

	endwin();
	return 0;
}

