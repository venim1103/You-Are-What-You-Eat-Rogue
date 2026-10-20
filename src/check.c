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

Entity *CheckNextChar(Level *leveli, char c, int type, int y, int x, Level level, Entity *monster)
{
	if(type==PLAYER)
	{
		// 1. Check if an active monster is at target tile:
		Entity *target = GetMonsterAt(y, x, level);
		if(target != NULL && target->alive == YES)
		{
			Combat(leveli, level, level.player, target, PLAYER);
			return monster;
		}

		// 2. Otherwise check terrain:
		chtype ch = mvinch(y, x) & A_CHARTEXT;
		level.player->Position.c = ch;

		switch(ch)
		{
			case '.':
			case '#':
				StepOn(leveli,PLAYER,y,x,ch,c,level,monster);
				level.player->Position.type=GROUND;
			break;
			case '+':
				OpenDoor(level.player, y, x);
			break;
			case ':':
				StepOn(leveli,PLAYER,y,x,ch,c,level,monster);
				level.player->Position.type=DOOR;
			break;
			case '>':
				StepOn(leveli,PLAYER,y,x,ch,c,level,monster);
				AskStairs(USTAIRS,level.player);
			break;
			case '<':
				StepOn(leveli,PLAYER,y,x,ch,c,level,monster);
				AskStairs(DSTAIRS,level.player);
			break;
			case '%':
				StepOn(leveli,PLAYER,y,x,ch,c,level,monster);
				level.player->Position.type=LOOT;
				AskEatCorpse(leveli, level, level.player, y, x);
			break;
			default:
				move(level.player->Position.yPos,level.player->Position.xPos);
				level.player->Position.type=UNKNOWN;
			break;
		}
	} else if(type==MONSTER)
	{
		if(monster == NULL || monster->alive != YES)
		{
			return monster;
		}

		// 1. Check if stepping into player:
		if(y == level.player->Position.yPos && x == level.player->Position.xPos)
		{
			Combat(leveli, level, level.player, monster, MONSTER);
			return monster;
		}

		// 2. Check if stepping into another alive monster:
		Entity *other = GetMonsterAt(y, x, level);
		if(other != NULL && other != monster && other->alive == YES)
		{
			monster->Position.type = UNKNOWN;
			return monster;
		}

		// 3. Otherwise check terrain:
		chtype ch = mvinch(y, x) & A_CHARTEXT;
		monster->Position.c = ch;

		switch(ch)
		{
			case '.':
			case '#':
			case '%':
				StepOn(leveli,MONSTER,y,x,ch,c,level,monster);
				monster->Position.type=GROUND;
			break;
			case ':':
				StepOn(leveli,MONSTER,y,x,ch,c,level,monster);
				monster->Position.type=DOOR;
			break;
			default:
				monster->Position.type=UNKNOWN;
			break;
		}
	}

	return monster;
}

int StepOn(Level *leveli, int type, int nexty, int nextx, char c, char symbol, Level level, Entity *monster)
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
	
			default: // Print the previous character and only after that update
				level.player = PlayerMove(level.player,buf,symbol,nexty,nextx);
				level.player->Position.ch = c;
			break;
		}
	} else if(type==MONSTER)
	{  
		y = monster->Position.yPos; 
		x = monster->Position.xPos;
		char restore_ch;

		if(monster->Position.type==DOOR)
		{
			restore_ch = ':';
		}
		else if(level.map_backup != NULL && level.map_backup[y][x] != '\0')
		{
			restore_ch = level.map_backup[y][x];
		}
		else
		{
			restore_ch = '.';
		}

		if(c == ':')
		{
			monster = MonsterMove(level, monster, restore_ch, symbol, nexty, nextx, DOOR);
		}
		else
		{
			monster = MonsterMove(level, monster, restore_ch, symbol, nexty, nextx, YES);
		}
	}
	return 1;
}

Entity *GetMonsterAt(int MonY, int MonX, Level level)
{
	for(int i=0; i<level.mons; i++)
	{
		if(level.monsters[i]->alive == YES &&
		   MonY==level.monsters[i]->Position.yPos && 
		   MonX==level.monsters[i]->Position.xPos)
		{
			return level.monsters[i];
		} 
	}
	return NULL;
}
