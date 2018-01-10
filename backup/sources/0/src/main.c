#include "rogue.h"

// Functions
int main(void) 
{

// Variables
	int c = 0;
	Entity *player; 
	Entity rooms; 
	char **map; 
	int x = 80; int y = 40; 
	clock_t ct; ct = clock(); double clk = ct;
	srand(clk);
// Init
	ScreenInit();
	rooms = MapInit(rooms); 
	map = SaveMap(map,y,x);

	player = PlayerSetup(player, rooms); 

// MAIN LOOP
	while((c = getchar()) != 'Q') 
	{
		player->input = c; 
		HandleInput(player, map); 
	}
	
// Exit
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
	for(int i=0; i<y; i++)
	{
		free(map[i]);
	}
	free(map);	

	endwin();
	return 0;
}

int ScreenInit()
{
	initscr();
	noecho(); 
	refresh(); 

	return 1;
}

