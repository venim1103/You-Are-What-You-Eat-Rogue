#include "global.h"

Position CheckArea(Position pos, char c)
{
	int x,y,w,h=2;
	x=pos.xPos; y=pos.yPos; w=pos.width; h=pos.height;

	for(int i=(y-1); i<((h+1)+y); i++)
	{
		for(int j=(x-1); j<((w+1)+x); j++)
		{
			if(mvinch(i,j)==c)
			{
				pos.type = YES;
			}
			else 
			{
				pos.type = NO;
				return pos; 
			}
		}
	}

	pos.xPos=x; pos.yPos=y;
	return pos;
}

Entity *CheckNextChar(char c, int type, int y, int x, Level level, Entity *monster)
{
	if(type==PLAYER)
	{
		level.player->Position.c = mvinch(y,x);

		switch(level.player->Position.c)
		{
			case '.':
			case '#':
				StepOn(PLAYER,y,x,level.player->Position.c,c,level,monster);
				level.player->Position.type=GROUND;
			break;
			case '+':
			case ':':
				StepOn(PLAYER,y,x,level.player->Position.c,c,level,monster);
				level.player->Position.type=DOOR;
			break;
			case '>':
				StepOn(PLAYER,y,x,level.player->Position.c,c,level,monster);
				AskStairs(USTAIRS,level.player);
			break;
			case '<':
				StepOn(PLAYER,y,x,level.player->Position.c,c,level,monster);
				AskStairs(DSTAIRS,level.player);
			break;
			default:
				move(level.player->Position.yPos,level.player->Position.xPos);
				level.player->Position.type=UNKNOWN;
			break;
		}
	} else if(type==MONSTER)
	{
		monster->Position.c = mvinch(y,x);

		switch(monster->Position.c)
		{
			case '.':
			case '#':
				StepOn(MONSTER,y,x,monster->Position.c,c,level,monster);
				monster->Position.type=GROUND;
			break;
			case '+':
			case ':':
				StepOn(MONSTER,y,x,monster->Position.c,c,level,monster);
				monster->Position.type=DOOR;
			break;
			case '@':
				StepOn(MONSTER,y,x,monster->Position.c,c,level,monster);
				monster->Position.type=ENEMY;
			break;
			default:
				monster->Position.type=UNKNOWN;
			break;
		}
	} else {}


	return monster;
}

int StepOn(int type, int nexty, int nextx, char c, char symbol, Level level, Entity *monster)
{
	int y,x = 0; 
	char buf[8];

	if(type==PLAYER)
	{
		sprintf(buf,"%c",level.player->Position.ch); // The character on last position
		switch(c)
		{

			case '+': // If there is a closed door:
				OpenDoor(level.player, nexty, nextx);
			break;
	
			// .. more here such as enemies and loot!

			default: // Print the previous character and only after that update
				level.player = PlayerMove(level.player,buf,symbol,nexty,nextx);

			break;
		}
	} else if(type==MONSTER)
	{
		sprintf(buf,"%c",c); // The character on last position
		switch(c)
		{
			case '.':
			case '#':
				if(monster->Position.type==DOOR)
				{
				y = monster->Position.yPos, x = monster->Position.xPos;
				PrintChar(y,x,':');
				monster->Position.yPos = nexty;monster->Position.xPos = nextx;
				y = monster->Position.yPos, x = monster->Position.xPos;
				PrintChar(y,x,monster->symbol);	
				} else
				{
					monster = MonsterMove(monster,buf,symbol,nexty,nextx);
				}
			break;
			case ':': // If there is an open door COPY FROM ORIGINAL MAP BACKUP!
				y = monster->Position.yPos, x = monster->Position.xPos;
				PrintChar(y,x,level.map_backup[y][x]);
				monster->Position.yPos = nexty;monster->Position.xPos = nextx;
				y = monster->Position.yPos, x = monster->Position.xPos;
				PrintChar(y,x,monster->symbol);	
			break;
			case '+': // If there is a closed door:
			break;
			case '@': // If there is the player:
			break;
	
			// .. more here such as enemies and loot!

			default: // Print the previous character and only after that update
				monster = MonsterMove(monster,buf,symbol,nexty,nextx);
			break;
		}
	}
	return 1;
}

