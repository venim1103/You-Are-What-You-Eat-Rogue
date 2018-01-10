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
		level.player->Position.c = mvinch(y,x);

		switch(level.player->Position.c)
		{
			case '.':
			case '#':
				StepOn(leveli,PLAYER,y,x,level.player->Position.c,c,level,monster);
				level.player->Position.type=GROUND;
			break;
			case '+':
			case ':':
				StepOn(leveli,PLAYER,y,x,level.player->Position.c,c,level,monster);
				level.player->Position.type=DOOR;
			break;
			case '>':
				StepOn(leveli,PLAYER,y,x,level.player->Position.c,c,level,monster);
				AskStairs(USTAIRS,level.player);
			break;
			case '<':
				StepOn(leveli,PLAYER,y,x,level.player->Position.c,c,level,monster);
				AskStairs(DSTAIRS,level.player);
			break;
			case 's':
			case 'g':
			case 'x':
			case 'T':	// ATTACK ONLY WHEN NEEDED TO AVOID CRASHING!!!!
				if(y==level.player->Position.yPos && x==level.player->Position.xPos)
				{
//					mvprintw(0,0,"\n");
//					mvprintw(0,0,"HERE !!!!!!!");
//					refresh(); getchar();
				} else
				{
					Combat(leveli, level,level.player,GetMonsterAt(y,x,level),PLAYER);
				}
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
				StepOn(leveli,MONSTER,y,x,monster->Position.c,c,level,monster);
				monster->Position.type=GROUND;
			break;
			case '+':
			case ':':
				StepOn(leveli,MONSTER,y,x,monster->Position.c,c,level,monster);
				monster->Position.type=DOOR;
			break;
			case '@':
			case 's':
			case 'g':
			case 'x':
			case 'T':	// ATTACK ONLY IF THERE IS A PLAYER THERE!!!
				if(y==level.player->Position.yPos && x==level.player->Position.xPos)
				{
					StepOn(leveli,MONSTER,y,x,monster->Position.c,c,level,monster);
				}
			break;
			default: // IF STOPPED!!!!
				monster->Position.type=UNKNOWN;
			break;
		}
	} else {}


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
	
			// .. more here such as enemies and loot!

			default: // Print the previous character and only after that update
				level.player = PlayerMove(level.player,buf,symbol,nexty,nextx);

			break;
		}
	} else if(type==MONSTER)
	{  
		switch(c)
		{
			case '.':
			case '#':	// FOR MAKING SURE THE OPEN DOOR WILL BE SAVED:
				if(monster->Position.type==DOOR)
				{
					monster = MonsterMove(level,monster,':',monster->symbol,nexty,nextx,YES);
				} else if(monster->Position.type==UNKNOWN) // IF STOPPED
				{
					y = monster->Position.yPos; x = monster->Position.xPos;
					if(level.map_backup[y][x]=='+')
					{
						c=':';
						level.map_backup[y][x]=c;
					} else
					{
						c=level.map_backup[y][x];	
					}
					monster = MonsterMove(level,monster,c,monster->symbol,nexty,nextx,YES);
				} else // IF NORMAL GROUND:
				{ 
					monster = MonsterMove(level,monster,c,symbol,nexty,nextx,YES);
				}
			break;
			case ':': // If there is an open door COPY FROM ORIGINAL MAP BACKUP!
				monster = MonsterMove(level,monster,c,symbol,nexty,nextx,DOOR);
			break;
			case '+': // If there is a closed door:
			break;
			case '@':
			case 's':
			case 'g':
			case 'x':
			case 'T':
				// AS THERE WILL BE SPACE LEFT BETWEEN MONSTER AND PLAYER:
				monster = MonsterMove(level,monster,c,symbol,nexty,nextx,FOLLOW);
				Combat(leveli,level,level.player,GetMonsterAt(monster->Position.yPos,monster->Position.xPos,level),MONSTER);
			break;
			// .. more here such as enemies and loot!

			default: // Print the previous character and only after that update
				monster = MonsterMove(level,monster,c,symbol,nexty,nextx,YES);
			break;
		}
	}
	return 1;
}

Entity *GetMonsterAt(int MonY, int MonX, Level level)
{
	for(int i=0; i<level.mons; i++)
	{
		if(MonY==level.monsters[i]->Position.yPos && MonX==level.monsters[i]->Position.xPos)
		{
			return level.monsters[i];
		} 
	}
	return NULL;
}
