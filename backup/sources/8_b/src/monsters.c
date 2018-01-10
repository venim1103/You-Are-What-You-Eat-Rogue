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

	monster->Position.type = NO;
	monster->Position.ch = monster->symbol;

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
	// Save the initial character value!
	monster->Position.ch = mvinch(monster->Position.yPos,monster->Position.xPos);
	
	sprintf(buf,"%c",monster->symbol);
	mvprintw(monster->Position.yPos, monster->Position.xPos, buf);
	refresh();

	free(yx);

	return 1;
}

Entity *MonsterSet(Entity *monster, char *buf, char c, int nexty, int nextx)
{
	char ch[8];
	sprintf(ch,"%c",c);
	///// FOR FIRST MOVEMENT BUG FIXING!!:////// 
        if(monster->mode==YES) // As long as the there was one movement:
	{ // cause for the bug:
		mvprintw(monster->Position.yPos,monster->Position.xPos, buf);
	}
	////

	monster->Position.yPos=nexty; monster->Position.xPos=nextx;
	mvprintw(monster->Position.yPos, monster->Position.xPos, ch);

	monster->mode=YES; // AFTER FIRST RUN KEEP PRINTING  

	return monster;
}

int *MoveMonster(int *yx, char symbol, int nexty, int nextx, Level level, Entity *monster)
{
	if(monster->mode==RANDOM)
	{

	} else if(monster->mode==FOLLOW)
	{
		yx = PathFindFollow(yx,nexty,nextx,level.player->Position.yPos,level.player->Position.xPos,'.','#');
		nexty = yx[0]; nextx = yx[1];
	}
	CheckNextChar(symbol,MONSTER,nexty,nextx,level);

	return yx;
}

int *PathFindFollow(int *yx, int y1, int x1, int y2, int x2, char chk1, char chk2)
{

	if(abs((x1-1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1-1))==chk1))
		{ // Distance between step smaller AND there is chk1
			x1 = x1-1;
	} else if(abs((x1-1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1-1))==chk2))
		{ // If distance smaller AND there is chk2
			x1 = x1-1;
	} else if(abs((x1+1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1+1))==chk1))
		{
			x1 = x1+1;
	} else if(abs((x1+1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1+1))==chk2))
		{
			x1 = x1+1;
	} else if(abs((y1-1)-y2)<abs(y1-y2)&&(mvinch((y1-1),x1)==chk1))
		{
			y1 = y1-1;
	} else if(abs((y1-1)-y2)<abs(y1-y2)&&(mvinch((y1-1),x1)==chk2))
		{
			y1 = y1-1;
	} else if(abs((y1+1)-y2)<abs(y1-y2)&&(mvinch((y1+1),x1)==chk1))
		{
			y1 = y1+1;
	} else if(abs((y1+1)-y2)<abs(y1-y2)&&(mvinch((y1+1),x1)==chk2))
		{
                        y1 = y1+1;
	}

	yx[0]=y1; yx[1]=x1;

	return yx;
}
