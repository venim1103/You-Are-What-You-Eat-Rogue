#include "global.h"

int *HandleInput(Level level, int *yx)
{
	// Save the original position values:
	int y = level.player->Position.yPos;
	int x = level.player->Position.xPos;

	

////////// TESTING /////////////
/*	if(level.player->mode==NO)
	{
		PrintNum(level.mons,"level.mons=   .");
	}
*/
///////////////////////////////

	
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

/////////////// CHARACTER MOVEMENT! ///////////////////////
// After updating the position variable,first get the next character:	
	CheckNextChar('@',PLAYER,y,x,level); 
	
/////////////// MONSTER MOVEMENT! ///////////////////////
	for(int i=0; i<level.mons; i++)
	{
		// Save the original position values:
		y = level.monsters[i]->Position.yPos; x = level.monsters[i]->Position.xPos;
		// Do movement for each:
		yx = MoveMonster(yx,level.monsters[i]->symbol,y,x,level,level.monsters[i]);
	}
//////////////////////////////////////////////////////////

	move(level.player->Position.yPos,level.player->Position.xPos);
	refresh();
	return yx;
}

