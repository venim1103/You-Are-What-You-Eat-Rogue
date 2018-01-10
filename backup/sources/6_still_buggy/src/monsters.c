#include "global.h"

Entity **AddMonsters(Level level, Entity rooms, int lvl, Level *leveli)
{
	Entity **monsters;
	monsters = malloc(sizeof(Entity*)*(ROOMS/2)); // Half of the rooms might have mons.

	leveli->mons = 0;

	int k = 0;

	// Count up to all the rooms
	for(int i=0; i<rooms.mode; i++)
	{
		if((rand()%2) == YES) // If every 50% say YES!
		{
			monsters[k] = SelMonster(lvl); 
			MonSetup(monsters[k], rooms);
			k++;
		}
	}	

	leveli->mons = k;

	return monsters;
}

Entity *SelMonster(int lvl)
{
	int mon = 0;

		if(lvl<=10)
		{	
			mon = (rand()%2)+SPIDER; //Can be SPIDER or GOBLIN
		} else if(lvl<=20)
		{	
			mon = (rand()%2)+GOBLIN; //Can be GOBLIN or TROLL  
		} else if(lvl>=20)
		{
			mon = TROLL; // Only TROLLS
		} else
		{
			mon = SPIDER;
		}

	switch(mon)
	{
		case SPIDER:
			return CreateMon('x',2,1,SPIDER);
		break;	

		case GOBLIN:
			return CreateMon('g',5,3,GOBLIN);
		break;	

		case TROLL:
			return CreateMon('T',15,5,TROLL);
		break;

		default:
			return CreateMon('x',2,1,SPIDER);
		break;	
	}	
	return CreateMon('x',2,1,SPIDER);
}

Entity *CreateMon(char c, int hp, int att, int mode)
{
	Entity *monster;
	monster = malloc(sizeof(Entity));

	monster->symbol = c;
	monster->health = hp;
	monster->attack = att;
	monster->maxhp = hp;
	monster->mode = mode;

	return monster;
}

int MonSetup(Entity *monster, Entity room)
{
	int random = 0; random=(rand()%room.mode);
	char buf[8];

	Position test; test.width = NO; test.height = NO; test.type = NO;

	int *yx; yx=malloc(sizeof(int)*3); 
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
	
	sprintf(buf,"%c",monster->symbol);
	mvprintw(monster->Position.yPos, monster->Position.xPos, buf);
	refresh();

	free(yx);

	return 1;
}
