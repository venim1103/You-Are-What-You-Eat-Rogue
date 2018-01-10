#include "global.h"

Level RedoLvl(Entity *player, Entity rooms, char **map)
{
			Level level;

			for(int i=0; i<Y_SCR; i++)
			{
				mvprintw(i,0,"\n");
			}
			FreeAll(player,rooms,map);
			level.rooms = MapInit(rooms);
			level.map = SaveMap(map,Y_SCR,X_SCR);
			level.player = PlayerSetup(player, level.rooms);
			// Make sure the upstairs by updating map:
			level.map = UpdateMap(level.map,Y_SCR,X_SCR);
			move(level.player->Position.yPos,level.player->Position.xPos);
			refresh();

			return level;
}
