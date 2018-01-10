#include "rogue.h"

char **SaveMap(char **XY, int y, int x)
{
	
	XY = malloc(sizeof(char*)*y); // Y-coordinate allocating

	for(int i=0; i<y; i++)
	{
		XY[i] = malloc(sizeof(char)*x); // X-coordinate allocating
		for(int j=0; j<x; j++)
		{
			XY[i][j]=mvinch(i,j);	
		}
	}
	return XY;
}

char **UpdateMap(char **map, int y, int x)
{
	for(int i=0; i<y; i++)
	{
		for(int j=0; j<x; j++)
		{
			map[i][j]=mvinch(i,j);	
		}
	}
	return map;
}

