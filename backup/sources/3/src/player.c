#include "global.h"

Entity *PlayerSetup(Entity *player, Entity room)
{
	player = malloc(sizeof(Entity));

	player->Position.type = 0;

	// Take random room
	int randomroom = 0; randomroom = (rand()%room.i);

	// Put somewhere randomly
	int *yx; int x,y=3;	// init random coords
	yx = RandomCoords(room.i,room.Area[randomroom]);
	y = yx[0]; x = yx[1];
	free(yx); // free the used random location pointer
	
	// FOR INITIAL MOVEMENT BUG:
	player->i = 0;

	// Set original position for the player
	player->Position.xPos = x; 
	player->Position.yPos = y;
	player->health = 20;

	// Print upstairs and downstairs + update the cursor
	PrintChar(player->Position.yPos,player->Position.xPos,'>');
	int room_old = randomroom;

	randomroom = (rand()%room.i); 
	while(randomroom==room_old)
	{
		randomroom = (rand()%room.i); 
	}
	// Put somewhere random	
	yx = RandomCoords(room.i,room.Area[randomroom]);
	y = yx[0]; x = yx[1];
	free(yx);
	
	PrintChar(y,x,'<');		

	
	move(player->Position.yPos,player->Position.xPos);
	
	// WELCOME TEXT + INSTRUCTIONS:
	mvprintw(0,0, "Welcome! Press 'Q' to exit and move with wasd+qezx");
	getch();
	refresh(); mvprintw(0,0, "\n"); // Clear the line


	return player;
}
