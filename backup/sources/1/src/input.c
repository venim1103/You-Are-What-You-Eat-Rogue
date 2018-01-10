#include "rogue.h"

int HandleInput(Entity *player, char **map)
{
	// Save the original position values:
	int y = player->Position.yPos;
	int x = player->Position.xPos;

	switch(player->input)
	{	// Move up, update the position variable accordingly:
		case 'w':
			y = player->Position.yPos-1;
			x = player->Position.xPos;
		break;
		// Move down
		case 's':
			y = player->Position.yPos+1;
			x = player->Position.xPos;
		break;
		// Move back
		case 'a':
			y = player->Position.yPos;
			x = player->Position.xPos-1;
		break;
		// Move forward
		case 'd':
			y = player->Position.yPos;
			x = player->Position.xPos+1;
		break;
		// Move upleft
		case 'q':
			y = player->Position.yPos-1;
			x = player->Position.xPos-1;
		break;
		// Move upright
		case 'e':
			y = player->Position.yPos-1;
			x = player->Position.xPos+1;
		break;
		// Move downleft
		case 'z':
			y = player->Position.yPos+1;
			x = player->Position.xPos-1;
		break;
		// Move downright
		case 'x':
			y = player->Position.yPos+1;
			x = player->Position.xPos+1;
		break;

		// Close door:
		case 'c': 
			TurnNearby(y,x,':',"+");
		break;
	
//		case 'u': 
//			UpdateMap(map,MAX_Y,MAX_X);
//		break;
	
		default:
		break;
	}

// After updating the position variable,first get the next character:	
	player->Position.c = mvinch(y,x);
	MovePlayer(y,x, player, map); 
	move(player->Position.yPos,player->Position.xPos);

	refresh();
	return 1;
}

