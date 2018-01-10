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
		clear();
		refresh();
		free(level.monsters[i]);
		PrintNum(level.mons,"level.mons:  .",14,12);
	}
	free(level.monsters);

	free(level.player);

	// Free the allocated door space!
	for(int i = 0; i<level.rooms.mode; i++)
	{
		free(level.rooms.Object[i]); 
	}	
	free(level.rooms.Object); 
	free(level.rooms.Area);
	// Free map screenshot
	for(int i=0; i<Y_SCR; i++)
	{
		free(level.map[i]);
	}
	free(level.map);	

	return 1;
}

