#include "global.h"

// Functions
int main(void) 
{

// CREATE THE LEVEL POINTER
	Level *level=NULL;

// Seed the random
	srand((unsigned int)time(NULL));
// Init
	ScreenInit();

// MAIN LOOP
	level = Game(level);

// Exit
	// level[0].maxlvl is the last created lvl id number
	ExitScreen(level);

	FreeAll(level, level->lvl, level->maxlvl);

	endwin();
	return 0;
}

