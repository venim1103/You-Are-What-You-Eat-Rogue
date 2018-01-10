#include "global.h"

int FreeAll(Entity *player, Entity rooms, char **map)
{
	// Free allocations:
	free(player);
	free(rooms.Area);
	// Free the allocated door space!
	for(int i = 0; i<rooms.i; i++)
	{
		free(rooms.Object[i]); 
	}	
	free(rooms.Object); 
	// Free map screenshot
	for(int i=0; i<Y_SCR; i++)
	{
		free(map[i]);
	}
	free(map);	

	return 1;
}

