#include "global.h"

int FreeAll(Level *level, int lvls)
{
	for(int i=0; i<=lvls; i++)
	{
		Free(level[i]);
	}
	free(level);
	return 1;
}

int Free(Level level)
{
	// Free allocations:
	
	// Free the allocated monsters
	for(int i=0; i<level.mons; i++)
	{
		free(level.monsters[i]);
	}
	free(level.monsters);

	free(level.player);
	free(level.rooms.Area);

	// Free the allocated door space!
	for(int i = 0; i<level.rooms.mode; i++)
	{
		free(level.rooms.Object[i]); 
	}	
	free(level.rooms.Object); 
	// Free map screenshot
	for(int i=0; i<Y_SCR; i++)
	{
		free(level.map[i]);
	}
	free(level.map);	

	return 1;
}

