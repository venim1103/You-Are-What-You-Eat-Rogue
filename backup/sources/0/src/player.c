#include "rogue.h"

Entity *PlayerSetup(Entity *player, Entity room)
{
	player = malloc(sizeof(Entity));

	// Set original position for the player
	player->Position.xPos = (room.Area[0].xPos)+1; 
	player->Position.yPos = (room.Area[0].yPos)+1;
	player->health = 20; player->Position.type = FLOOR;

	// Print player and update the cursor
	mvprintw(player->Position.yPos, player->Position.xPos, "@");
	move(player->Position.yPos, player->Position.xPos);
	
	// WELCOME TEXT + INSTRUCTIONS:
	mvprintw(0,0, "Welcome! Press 'Q' to exit and move with wasd+qezx");
	refresh(); mvprintw(0,0, "\n"); // Clear the line
	getchar();

	return player;
}

int MovePlayer(int y, int x, Entity *player, char **map)
{
	switch(player->Position.c)
	{
		case '.':
			if(player->Position.type==FLOOR)
			{
				StepOn(y,x,'.',player, map);
				player->Position.type=FLOOR;
			}
			else if(player->Position.type==DOOR)
			{
				StepOn(y,x,':',player, map);
				player->Position.type=FLOOR; 
			}
		break;
		
		case ':': 
			StepOn(y,x,'.',player, map);
			player->Position.type=DOOR;
		break;
		
		case '+': 
			StepOn(y,x,'+',player, map);
			player->Position.type=FLOOR; 
		break;

		default:
			move(player->Position.yPos, player->Position.xPos);
		break;
	}
	return 1;
}

