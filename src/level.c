#include "global.h"

Level MakeLvl(Level level_in, int lvl, Level *leveli)
{
//			Level level;

/*			for(int i=0; i<Y_SCR; i++)
			{
				mvprintw(i,0,"\n");
			}
*/
			clear(); // ncurses CLEAR THE SCREEN!

			level_in.rooms = MapInit(level_in.rooms);
			level_in.map = SaveMap(level_in.map,Y_SCR,X_SCR);

			level_in.map_backup = SaveMap(level_in.map,Y_SCR,X_SCR);

			level_in.player = PlayerSetup(leveli, level_in.player, level_in.rooms);

			level_in.mons = 0;
			level_in.max_corpses = 64;
			level_in.num_corpses = 0;
			level_in.corpses = malloc(sizeof(Corpse) * level_in.max_corpses);
			for(int i = 0; i < level_in.max_corpses; i++)
			{
				level_in.corpses[i].active = NO;
				level_in.corpses[i].age = 0;
			}

			level_in.lvl = lvl;
			level_in.monsters = AddMonsters(&level_in, level_in.rooms, lvl);
			// Draw player symbol on initial tile (while keeping '>' in map_backup and Position.ch):
			level_in.player->mode = YES;
			PrintChar(level_in.player->Position.yPos, level_in.player->Position.xPos, leveli->player_symbol);
			// Make sure the upstairs by updating map:
			level_in.map = UpdateMap(level_in.map,Y_SCR,X_SCR);
			move(level_in.player->Position.yPos,level_in.player->Position.xPos);
			refresh();

			return level_in;
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
