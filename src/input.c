#include "global.h"

int *HandleInput(Level *leveli, Level level, int *yx)
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
			mvprintw(0,0,"\n");
			mvprintw(0,0,"You are closing nearby doors!");
			refresh();
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
	Entity *empty = NULL;
	CheckNextChar(leveli, leveli->player_symbol,PLAYER,y,x,level,empty); 
	
/////////////// MONSTER MOVEMENT! ///////////////////////
	char chk[10] = {'.','#',':','@','s', 'g','x','T'};
//	char *chk = chks;
	for(int i=0; i<level.mons; i++)
	{
		// Save the original position values:
		y = level.monsters[i]->Position.yPos; x = level.monsters[i]->Position.xPos;
		// Do movement for each:

		if(level.monsters[i]->type==FOLLOW)
		{
			yx = PathFindFollow(yx,y,x,level.player->Position.yPos,level.player->Position.xPos, chk);
		} else if(level.monsters[i]->mode==RANDOM_S)
		{
			yx = PathFindRandom(yx,y,x,chk,RANDOM_S);
		} else if(level.monsters[i]->mode==RANDOM_X)
		{
			yx = PathFindRandom(yx,y,x,chk,RANDOM_X);
		} else
		{
			yx = PathFindRandom(yx,y,x,chk,RANDOM_X);
		}
		
		y = yx[0]; x = yx[1];
		
		level.monsters[i] = CheckNextChar(leveli,level.monsters[i]->symbol,MONSTER,y,x,level,level.monsters[i]);
	}
//////////////////////////////////////////////////////////

	move(level.player->Position.yPos,level.player->Position.xPos);
	refresh();
    
	return yx;
}




