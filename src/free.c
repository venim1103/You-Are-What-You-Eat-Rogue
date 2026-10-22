#include "global.h"

int FreeAll(Level *level, int lvl, int lvls)
{
	for(int i=0; i<=lvls; i++)
	{
		Free(level[i], lvl, lvls);
	}
	free(level);
	return 1;
}

int Free(Level level, int lvl, int lvls)
{
	// Free allocations:

	// Free the allocated monsters
	for(int i=0; i<level.mons; i++)
	{
		free(level.monsters[i]);
	}
	free(level.monsters);
	if(level.corpses != NULL)
	{
		free(level.corpses);
	}
	if(level.items != NULL)
	{
		free(level.items);
	}
	free(level.player);

	// Free the allocated door space!
	for(int i = 0; i<level.rooms.mode; i++)
	{
		free(level.rooms.Object[i]); 
	}	
	free(level.rooms.Object); 
	free(level.rooms.Area);
	// Free map screenshot and FOV arrays
	for(int i=0; i<Y_SCR; i++)
	{
		free(level.map[i]);
		free(level.map_backup[i]);
		if(level.discovered != NULL) free(level.discovered[i]);
		if(level.visible != NULL) free(level.visible[i]);
	}
	free(level.map);	
	free(level.map_backup);	
	if(level.discovered != NULL) free(level.discovered);
	if(level.visible != NULL) free(level.visible);

	return 1;
}

