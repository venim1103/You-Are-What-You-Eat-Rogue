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
			player->Position.type=GROUND;
		break;
		case ':':
			StepOn(y,x,player->Position.c,player,map);
			player->Position.type=DOOR;
		break;
		case '#':
			StepOn(y,x,player->Position.c,player,map);
			player->Position.type=GROUND;
		break;
		case '+':
			StepOn(y,x,player->Position.c,player,map);
			player->Position.type=DOOR;
		break;
		case '>':
			StepOn(y,x,player->Position.c,player,map);
			AskStairs(USTAIRS,player);
		break;
		case '<':
			StepOn(y,x,player->Position.c,player,map);
			AskStairs(DSTAIRS,player);
		break;
		default:
			move(player->Position.yPos,player->Position.xPos);
			player->Position.type=UNKNOWN;
		break;
	}
	return 1;
}

int StepOn(int nexty, int nextx,char c, Entity *player, char **map)
{

	char *buf; buf = malloc(sizeof(char)*8);
	sprintf(buf,"%c",player->Position.ch); // The character on last position

	switch(c)
	{

	case '+': // If there is a closed door:
		OpenDoor(player, nexty, nextx);
	break;
	
	// .. more here such as enemies and loot!

	default: // Print the previous character and only after that update
		player = PlayerMove(player,buf,nexty,nextx);

	break;
	}
	
	free(buf);	
	return 1;
}

