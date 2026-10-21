#include "global.h"

Entity **AddMonsters(Level *level, Entity rooms, int lvl)
{
	int makeamount = 0;
	makeamount = ((ROOMS/2)+lvl);

	Entity **monsters;
	monsters = malloc(sizeof(Entity*)*(makeamount+1)); // Half of the rooms might have mons.
	int *yx; yx=malloc(sizeof(int)*3); 

	level->mons = 0;

	int k = 0;

	// Count up to all the rooms
	for(int i=0; i<makeamount; i++)
	{
		if((rand()%2) == YES) // If every 50% say YES!
		{
			monsters[k] = SelMonster(lvl); 
			yx = MonSetup(monsters[k], rooms, yx);
			k++;
		}
	}	

	level->mons = k;

	free(yx);
	
	return monsters;
}

Entity *SelMonster(int lvl)
{
	int mon = SPIDER;

	if(lvl <= 2)
	{
		// Floors 0-2: Spider, Snake, Bat
		int r = rand() % 3;
		if(r == 0) mon = SPIDER;
		else if(r == 1) mon = SNAKE;
		else mon = BAT;
	}
	else if(lvl <= 6)
	{
		// Floors 3-6: Bat, Snake, Goblin, Slime
		int r = rand() % 4;
		if(r == 0) mon = BAT;
		else if(r == 1) mon = SNAKE;
		else if(r == 2) mon = GOBLIN;
		else mon = SLIME;
	}
	else if(lvl <= 12)
	{
		// Floors 7-12: Goblin, Slime, Ghost, Troll
		int r = rand() % 4;
		if(r == 0) mon = GOBLIN;
		else if(r == 1) mon = SLIME;
		else if(r == 2) mon = GHOST;
		else mon = TROLL;
	}
	else
	{
		// Floors 13+: High danger (Troll, Ghost, Slime, occasional Bat/Goblin)
		int roll = rand() % 100;
		if(roll < 40) mon = TROLL;
		else if(roll < 65) mon = GHOST;
		else if(roll < 85) mon = SLIME;
		else if(roll < 95) mon = GOBLIN;
		else mon = BAT;
	}

	switch(mon)
	{
		case SPIDER:
			return CreateMon('x', 2, 1, SPIDER, RANDOM_X, "Spider");
		break;	
		
		case SNAKE:
			return CreateMon('s', 3, 2, SNAKE, RANDOM_S, "Snake");
		break;	

		case BAT:
			return CreateMon('V', 2, 2, BAT, RANDOM_X, "Vampire Bat");
		break;

		case GOBLIN:
			return CreateMon('g', 5, 3, GOBLIN, FOLLOW, "Goblin");
		break;	

		case SLIME:
			return CreateMon('B', 8, 2, SLIME, RANDOM_S, "Acid Slime");
		break;

		case GHOST:
			return CreateMon('G', 4, 3, GHOST, FOLLOW, "Ghost");
		break;
		
		case TROLL:
			return CreateMon('T', 15, 5, TROLL, FOLLOW, "Troll");
		break;

		default:
			return CreateMon('g', 5, 3, GOBLIN, FOLLOW, "Goblin");
		break;	
	}	
	return CreateMon('T', 15, 5, TROLL, FOLLOW, "Troll");
}

Entity *CreateMon(char c, int hp, int att, int mode, int type, char *name)
{
	Entity *monster;
	monster = malloc(sizeof(Entity));

	monster->symbol = c;
	monster->health = hp;
	monster->attack = att;
	monster->maxhp = hp;
	monster->mode = mode;
	monster->type = type;
	monster->name = name;

	monster->Position.type = NO;
	monster->alive = YES;
	monster->poison_turns = 0;
	ApplyFormTraits(monster, c);

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
	mvprintw(monster->Position.yPos, monster->Position.xPos, "%s", buf);
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

