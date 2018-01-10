#include "global.h"

Position CheckArea(Position pos)
{
	int x,y,w,h=2;
	x=pos.xPos; y=pos.yPos; w=pos.width; h=pos.height;

	for(int i=(y-1); i<((h+1)+y); i++)
	{
		for(int j=(x-1); j<((w+1)+x); j++)
		{
			if(mvinch(i,j)==' ')
			{
				pos.type = 1;
			}
			else 
			{
				pos.type = 0;
				return pos; 
			}
		}
	}

	pos.xPos=x; pos.yPos=y;
	return pos;
}

int CheckNextChar(int y, int x, Entity *player, char **map)
{
	switch(player->Position.c)
	{
		case '.':
			StepOn(y,x,player->Position.c,player,map);
		break;
		case ':':
			StepOn(y,x,player->Position.c,player,map);
		break;
		case '#':
			StepOn(y,x,player->Position.c,player,map);
		break;
		case '+':
			StepOn(y,x,player->Position.c,player,map);
		break;
		case '>':
			StepOn(y,x,player->Position.c,player,map);
		break;
		default:
			move(player->Position.yPos,player->Position.xPos);
		break;
	}
	return 1;
}

int StepOn(int nexty, int nextx,char c, Entity *player, char **map)
{

	char buf[8];
	sprintf(buf,"%c",player->Position.ch); // The character on last position

	switch(c)
	{

	case '+': // If there is a closed door:
		OpenDoor(player, nexty, nextx);
	break;
	// .. more here such as enemies and loot!

	default: // Print the previous character and only after that update
		///// FOR FIRST MOVEMENT BUG FIXING!!://////
		if(player->i==1) // As long as the there was one movement:
		{ // cause for the bug:
		mvprintw(player->Position.yPos,player->Position.xPos, buf);
		}
		////

		player->Position.yPos=nexty; player->Position.xPos=nextx;
		mvprintw(player->Position.yPos, player->Position.xPos, "@");
		move(player->Position.yPos, player->Position.xPos);
		
		player->i=1; // AFTER FIRST RUN KEEP PRINTING

	break;
	}	
	return 1;
}

