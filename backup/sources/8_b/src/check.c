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

int CheckNextChar(char c, int type, int y, int x, Level level)
{

	Entity *Empty = NULL;

	if(type==PLAYER)
	{
		level.player->Position.c = mvinch(y,x);

		switch(level.player->Position.c)
		{
			case '.':
			case '#':
				StepOn(PLAYER,y,x,level.player->Position.c,c,level,Empty);
				level.player->Position.type=GROUND;
			break;
			case '+':
			case ':':
				StepOn(PLAYER,y,x,level.player->Position.c,c,level,Empty);
				level.player->Position.type=DOOR;
			break;
			case '>':
				StepOn(PLAYER,y,x,level.player->Position.c,c,level,Empty);
				AskStairs(USTAIRS,level.player);
			break;
			case '<':
				StepOn(PLAYER,y,x,level.player->Position.c,c,level,Empty);
				AskStairs(DSTAIRS,level.player);
			break;
			default:
				move(level.player->Position.yPos,level.player->Position.xPos);
				level.player->Position.type=UNKNOWN;
			break;
		}
	} else if(type==MONSTER)
	{
		for(int i=0; i<level.mons; i++)
		{
			level.monsters[i]->Position.c = mvinch(y,x);

			switch(level.monsters[i]->Position.c)
			{
				case '.':
				case '#':
					StepOn(MONSTER,y,x,level.monsters[i]->Position.c,c,level,level.monsters[i]);
					level.monsters[i]->Position.type=GROUND;
				break;
				case '+':
				case ':':
					StepOn(MONSTER,y,x,level.monsters[i]->Position.c,c,level,level.monsters[i]);
					level.monsters[i]->Position.type=DOOR;
				break;
				case '@':
					StepOn(MONSTER,y,x,level.monsters[i]->Position.c,c,level,level.monsters[i]);
					level.monsters[i]->Position.type=ENEMY;
				break;
				default:
					level.monsters[i]->Position.type=UNKNOWN;
				break;
			}
		}
	} else {}

	return 1;
}

int StepOn(int type, int nexty, int nextx, char c, char symbol, Level level, Entity *monster)
{

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
		sprintf(buf,"%c",monster->symbol); // The character on last position
		switch(c)
		{
			case '+': // If there is a closed door:
			break;
			case '@': // If there is the player:
			break;
	
			// .. more here such as enemies and loot!

			default: // Print the previous character and only after that update
				monster = MonsterSet(monster,buf,symbol,nexty,nextx);

			break;
		}
	}
	return 1;
}

