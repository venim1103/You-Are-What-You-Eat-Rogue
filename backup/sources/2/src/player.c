#include "global.h"

Entity *PlayerSetup(Entity *player, Entity room)
{
	player = malloc(sizeof(Entity));

	// Take random room
	int randomroom = 0; randomroom = (rand()%room.i);

	// Put into middle
	int x = 2; x = ((room.Area[randomroom].xPos)+(room.Area[randomroom].width/2));
	int y = 2; y = ((room.Area[randomroom].yPos)+(room.Area[randomroom].height/2));

	// FOR INITIAL MOVEMENT BUG:
	player->i = 0;

	// Set original position for the player
	player->Position.xPos = x; 
	player->Position.yPos = y;
	player->health = 20;
	// Print upstairs and update the cursor
	PrintChar(player->Position.yPos,player->Position.xPos,'>');
	move(player->Position.yPos,player->Position.xPos);
	
	// WELCOME TEXT + INSTRUCTIONS:
	mvprintw(0,0, "Welcome! Press 'Q' to exit and move with wasd+qezx");
	getch();
	refresh(); mvprintw(0,0, "\n"); // Clear the line


	return player;
}
