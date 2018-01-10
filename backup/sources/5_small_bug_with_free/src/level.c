#include "global.h"

Level MakeLvl(Level level_in, int lvl, Level *leveli)
{
			Level level;

/*			for(int i=0; i<Y_SCR; i++)
			{
				mvprintw(i,0,"\n");
			}
*/
			clear(); // ncurses CLEAR THE SCREEN!

			level.rooms = MapInit(level_in.rooms);
			level.map = SaveMap(level_in.map,Y_SCR,X_SCR);
			level.player = PlayerSetup(level_in.player, level.rooms);

			level.mons = 0; leveli->mons = 0;
			level.monsters = AddMonsters(level_in, level.rooms, lvl, leveli);
			level.mons = leveli->mons;	

			// Make sure the upstairs by updating map:
			level.map = UpdateMap(level.map,Y_SCR,X_SCR);
			move(level.player->Position.yPos,level.player->Position.xPos);
			refresh();

			return level;
}

int *RandomCoords(int *yx, int rm, Position room)
{

//	yx = malloc(sizeof(int)*3);

	int x,y=3;

	x = ((room.xPos+1)+(rand()%(room.width-1)));
	y = ((room.yPos+1)+(rand()%(room.height-1)));

	yx[0] = y; yx[1] = x; 
	return yx; 
}
