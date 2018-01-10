#include "rogue.h"

// Functions
int main(void) 
{

	char buf[8];

// Variables
	char co = ' ';
	int c = 0;
	Entity *player; 
	Entity rooms; 
	char **map; 
	clock_t ct; ct = clock(); double clk = ct;
	srand(clk);
// Init
	ScreenInit();
	rooms = MapInit(rooms); 
	map = SaveMap(map,MAX_Y,MAX_X);

	player = PlayerSetup(player, rooms); 
// MAIN LOOP
	while((c = getchar()) != 'Q') 
	{

		player->input = c; 
		HandleInput(player, map);
		
		co = map[player->Position.yPos][player->Position.xPos];

		if(co!='@') // IF NOT STAY STILL SO THAT PREV CHAR is @:
		{
		player->Position.ch = co; // Update the last character
		}

//		sprintf(buf,"%c",player->Position.ch);
//		mvprintw(0,65,buf);

 
		UpdateMap(map,MAX_Y,MAX_X);
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
	for(int i=0; i<MAX_Y; i++)
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

