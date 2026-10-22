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
			level_in.max_mons = 64;
			level_in.monsters = AddMonsters(&level_in, level_in.rooms, lvl);
			// Spawn 2-4 floor items (weapons & armor):
			level_in.max_items = 32;
			level_in.num_items = 0;
			level_in.items = malloc(sizeof(Item) * level_in.max_items);
			for(int i = 0; i < level_in.max_items; i++)
			{
				level_in.items[i].active = NO;
				level_in.items[i].equipped = NO;
			}
			SpawnFloorItems(&level_in, level_in.rooms, lvl);

			// Allocate discovered and visible arrays for Fog of War:
			level_in.discovered = malloc(sizeof(char*) * Y_SCR);
			level_in.visible = malloc(sizeof(char*) * Y_SCR);
			for(int i = 0; i < Y_SCR; i++)
			{
				level_in.discovered[i] = malloc(sizeof(char) * X_SCR);
				level_in.visible[i] = malloc(sizeof(char) * X_SCR);
				for(int j = 0; j < X_SCR; j++)
				{
					level_in.discovered[i][j] = 0;
					level_in.visible[i][j] = 0;
				}
			}

			// Draw player symbol on initial tile (while keeping '>' in map_backup and Position.ch):
			level_in.player->mode = YES;
			PrintChar(level_in.player->Position.yPos, level_in.player->Position.xPos, leveli->player_symbol);
			level_in.map = UpdateMap(level_in.map,Y_SCR,X_SCR);
			move(level_in.player->Position.yPos,level_in.player->Position.xPos);

			UpdateFOV(&level_in);
			RenderFOV(&level_in);
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
