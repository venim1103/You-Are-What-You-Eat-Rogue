#include "global.h"

int HandleInput(Entity *player, char **map, Entity rooms)
{
	// Save the original position values:
	int y = player->Position.yPos;
	int x = player->Position.xPos;
	
	char num[16];

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
	
		case 'u':
			for(int i=0; i<rooms.mode; i++)
			{	
			mvprintw(0,0,"Room    width:    and height:   .");
			sprintf(num,"%d",i);
			mvprintw(0,5,num);
			sprintf(num,"%d",rooms.Area[i].width);
			mvprintw(0,15,num);
			sprintf(num,"%d",rooms.Area[i].height);
			mvprintw(0,30,num);

			getchar();

			refresh(); mvprintw(0,0,"\n");
 			}
		break;
		
		case '?':
			getchar();
			PrintMap(MAX_Y,MAX_X,map);
			refresh();
			getchar();	
		break;
		
		default:
		break;
	}

// After updating the position variable,first get the next character:	
	player->Position.c = mvinch(y,x);
	
	CheckNextChar(y,x, player, map); 

	move(player->Position.yPos,player->Position.xPos);

	refresh();
	return 1;
}

