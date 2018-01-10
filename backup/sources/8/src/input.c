#include "global.h"

int HandleInput(Level level)
{
	// Save the original position values:
	int y = level.player->Position.yPos;
	int x = level.player->Position.xPos;
	
	char num[16];

	switch(level.player->input)
	{	// Move up, update the position variable accordingly:
		case 'w':
			y = level.player->Position.yPos-1;
			x = level.player->Position.xPos;
		break;
		// Move down
		case 's':
			y = level.player->Position.yPos+1;
			x = level.player->Position.xPos;
		break;
		// Move back
		case 'a':
			y = level.player->Position.yPos;
			x = level.player->Position.xPos-1;
		break;
		// Move forward
		case 'd':
			y = level.player->Position.yPos;
			x = level.player->Position.xPos+1;
		break;
		// Move upleft
		case 'q':
			y = level.player->Position.yPos-1;
			x = level.player->Position.xPos-1;
		break;
		// Move upright
		case 'e':
			y = level.player->Position.yPos-1;
			x = level.player->Position.xPos+1;
		break;
		// Move downleft
		case 'z':
			y = level.player->Position.yPos+1;
			x = level.player->Position.xPos-1;
		break;
		// Move downright
		case 'x':
			y = level.player->Position.yPos+1;
			x = level.player->Position.xPos+1;
		break;

		// Close door:
		case 'c': 
			TurnNearby(y,x,':',"+");
		break;
	
		case 'u':
			for(int i=0; i<level.rooms.mode; i++)
			{	
			mvprintw(0,0,"Room    width:    and height:   .");
			sprintf(num,"%d",i);
			mvprintw(0,5,num);
			sprintf(num,"%d",level.rooms.Area[i].width);
			mvprintw(0,15,num);
			sprintf(num,"%d",level.rooms.Area[i].height);
			mvprintw(0,30,num);

			getchar();

			refresh(); mvprintw(0,0,"\n");
 			}
		break;
		
		case '?':
			getchar();
			PrintMap(MAX_Y,MAX_X,level.map);
			refresh();
			getchar();	
		break;
		
		default:
		break;
	}

// After updating the position variable,first get the next character:	
	CheckNextChar('@',PLAYER,y,x,level); 

	move(level.player->Position.yPos,level.player->Position.xPos);

	refresh();
	return 1;
}

