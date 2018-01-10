#include "global.h"

// Functions
int main(void) 
{

// Variables

	// CREATE 10 LVLS
	int lvls = 10;
	Level level[lvls]; 
	level[0].player=NULL; level[0].map=NULL; 

	for(int i=0; i<lvls; i++)
	{
		level[i].lvl=i;
	}

// Seed the random
	clock_t ct; ct = clock(); double clk = ct; srand(clk);
// Init
	ScreenInit();

// MAIN LOOP
	level[0] = Game(level[0]);	
// Exit
	FreeAll(level[0].player,level[0].rooms,level[0].map);
	endwin();
	return 0;
}

int ScreenInit()
{
	initscr();
	noecho(); 
	refresh(); 

	return 1;
}

Level Game(Level level)
{
	char co = ' ';
	int c = 0;

	level.rooms = MapInit(level.rooms); 
	level.map = SaveMap(level.map,Y_SCR,X_SCR);
	level.player = PlayerSetup(level.player, level.rooms);
	level.map = UpdateMap(level.map,Y_SCR,X_SCR);

	while((c = getchar()) != 'Q') 
	{
		if(c == 'r')
		{
			level=RedoLvl(level.player,level.rooms,level.map);
		}


		level.player->input = c; 
		HandleInput(level.player, level.map, level.rooms);
		
		co = level.map[level.player->Position.yPos][level.player->Position.xPos];

		if(co!='@') // IF NOT STAY STILL SO THAT PREV CHAR is @:
		{
		level.player->Position.ch = co; // Update the last character
		}
 
		level.map = UpdateMap(level.map,Y_SCR,X_SCR);
	}
	return level;
}
