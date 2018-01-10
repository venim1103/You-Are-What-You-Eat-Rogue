#include "global.h"

int ScreenInit()
{
	initscr();
	noecho(); 
	refresh(); 

	return 1;
}

Level *Game(Level *level)
{
	char co = ' ';
	int c,ri = 0;
	
	level = malloc(sizeof(Level)*(LVLS+1));

	// Make 1st level:
	level[0] = MakeLvl(level[0]);

	while((c = getchar()) != 'Q') 
	{
		level[ri].map = UpdateMap(level[ri].map,Y_SCR,X_SCR);
	
		if(c == 'r' || level[ri].player->Position.type==STAIRS)
		{
			if(ri>=LVLS)
			{
				mvprintw(0,0,"The staircase is blocked!! This is the last level!");
				getch();
				refresh();
				continue; // ignore after reaching limit
				
			}
			ri++;
			level[ri] = MakeLvl(level[ri]);
			level[ri].player->Position.type=0;
		}

		level[ri].player->input = c; 
		HandleInput(level[ri].player, level[ri].map, level[ri].rooms);
		
		co = level[ri].map[level[ri].player->Position.yPos][level[ri].player->Position.xPos];

		if(co!='@') // IF NOT STAY STILL SO THAT PREV CHAR is @:
		{
		level[ri].player->Position.ch = co;// Update the last character
		}
 
		level[ri].map = UpdateMap(level[ri].map,Y_SCR,X_SCR);
	}

	// For returning the last created lvl save up the value for id:
	level[0].maxlvl = ri;

	return level;
}
