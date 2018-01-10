#include "global.h"

Entity **AddMonsters(Level level, Entity rooms, int lvl, Level *leveli)
{
	Entity **monsters;
	monsters = malloc(sizeof(Entity*)*(ROOMS/2)); // Half of the rooms might have mons.
	int *yx; yx=malloc(sizeof(int)*3); 

	leveli->mons = 0;

	int k = 0;

	// Count up to all the rooms
	for(int i=0; i<rooms.mode; i++)
	{
		if((rand()%2) == YES) // If every 50% say YES!
		{
			monsters[k] = SelMonster(lvl); 
			yx = MonSetup(monsters[k], rooms, yx);
			k++;
		}
	}	

	leveli->mons = k;

	free(yx);
	
	return monsters;
}

Entity *SelMonster(int lvl)
{
	int mon = 0;

		if(lvl<=3)
		{	
			mon = (rand()%SNAKE)+1; //Can be SNAKE OR GOBLIN
		} else if(lvl<=5)
		{ if(lvl<22)
			{	
			mon = (rand()%GOBLIN)+1; //Can be SNAKE, GOBLIN, SPIDER  
			}
		} else if(lvl>=10)
		{ if(lvl<22)
			{
			mon = (rand()%TROLL)+1; //Can be SNAKE,GOBLIN,SPIDER,TROLL 
			}
		} else if(lvl>=22)
		{
			mon = TROLL; // Only TROLLS
		} else
		{
			mon = SNAKE;
		}

	switch(mon)
	{
		case SPIDER:
			return CreateMon('x',2,1,SPIDER, RANDOM_X);
		break;	
		
		case SNAKE:
			return CreateMon('s',3,2,SNAKE, RANDOM_S);
		break;	

		case GOBLIN:
			return CreateMon('g',5,3,GOBLIN, FOLLOW);
		break;	
		
		case TROLL:
			return CreateMon('T',15,5,TROLL, FOLLOW);
		break;

		default:
//			return CreateMon('x',3,2,SNAKE, RANDOM_S);
		break;	
	}	
	return CreateMon('T',15,5,TROLL, FOLLOW);
}

Entity *CreateMon(char c, int hp, int att, int mode, int type)
{
	Entity *monster;
	monster = malloc(sizeof(Entity));

	monster->symbol = c;
	monster->health = hp;
	monster->attack = att;
	monster->maxhp = hp;
	monster->mode = mode;
	monster->type = type;

	monster->Position.type = NO;
	monster->alive = YES;

	return monster;
}

int *MonSetup(Entity *monster, Entity room, int *yx)
{
	int random = 0; random=(rand()%room.mode);
	char buf[8];

	Position test; test.width = NO; test.height = NO; test.type = NO;

	for(int i=0; i<9999; i++)
	{
		if(test.type==NO)
		{
			random=(rand()%room.mode); 
			yx=RandomCoords(yx,room.mode,room.Area[random]);
			test.yPos = yx[0]; test.xPos = yx[1];
			test = CheckArea(test,'.');		 	
		} else
		{
			break;
		}
	}
	monster->Position.yPos = yx[0]; 
	monster->Position.xPos = yx[1]; 
	// Save the initial character value!
	monster->Position.ch = mvinch(monster->Position.yPos,monster->Position.xPos);
	monster->Position.c = mvinch(monster->Position.yPos,monster->Position.xPos);
	
	sprintf(buf,"%c",monster->symbol);
	mvprintw(monster->Position.yPos, monster->Position.xPos, buf);
	refresh();


	return yx;
}

Entity *MonsterMove(Level level, Entity *monster, char c1, char c2, int nexty, int nextx, int mode)
{
	int y,x = 0;
	y = monster->Position.yPos; x = monster->Position.xPos;

if(monster->alive==YES)
{
	if(mode==YES)
	{
		PrintChar(monster->Position.yPos,monster->Position.xPos, c1);
		monster->Position.yPos = nexty; monster->Position.xPos = nextx; y = nexty; x = nextx;
		PrintChar(y,x, c2);
	} else if(mode==DOOR)
	{
		PrintChar(y,x,level.map_backup[y][x]);
		monster->Position.yPos = nexty; monster->Position.xPos = nextx; y = nexty; x = nextx;
		PrintChar(y,x,monster->symbol);	
	} else if(mode==FOLLOW)
	{
		if(level.map_backup[y][x]=='+')
		{
			PrintChar(y,x,':');
			level.map_backup[y][x]=':';
		} else
		{
			PrintChar(y,x,level.map_backup[y][x]);
		} // AS THERE WILL BE SPACE LEFT BETWEEN MONSTER AND PLAYER:	
		if((nexty-y)==2)
		{
			y++;
		}	
		if((nexty-y)==-2)
		{
			y--;
		}	
		if((nextx-x)==2)
		{
			x++;
		}	
		if((nextx-x)==-2)
		{
			x--;
		}	
		monster->Position.yPos = y; monster->Position.xPos = x;
		PrintChar(y,x,monster->symbol);
	}
}
	return monster;
}

