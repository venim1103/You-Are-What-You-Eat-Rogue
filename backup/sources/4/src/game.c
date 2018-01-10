#include "global.h"

int ScreenInit()
{
	MAX_X = 80; MAX_Y = 40;
	X_SCR = MAX_X+10; Y_SCR = MAX_Y+10;
	LVLS = 100;

	initscr();
	noecho(); 
	refresh(); 

	return 1;
}

Level *Game(Level *level)
{
	char co = ' ';
	int c,r = 0;
	
	level = malloc(sizeof(Level)*(LVLS+1));

	// Make 1st level:
	level[0] = MakeLvl(level[0], 0); //level[0].lvl = 0;

	level->lvl = 0;
	level->maxlvl = 0;

	WelcomeTxt();

	while((c = getchar()) != 'Q') 
	{
		level[r].map = UpdateMap(level[r].map,Y_SCR,X_SCR);
	
		if(c == 'r' || level[r].player->Position.type==DSTAIRS)
		{
			level = Downstairs(level,r); 
			r = level->lvl;
			PrintMap(MAX_Y,MAX_X,level[r].map);
		}
		if(level[r].player->Position.type==USTAIRS)
		{
			level = Upstairs(level,r);
			r = level->lvl; 
			PrintMap(MAX_Y,MAX_X,level[r].map);
		}

		if(c=='P')
		{
		char num[16];
		mvprintw(0,0,"\n"); mvprintw(0,0,"LVL =   , MAXLVL =   ."); 		
		sprintf(num,"%d",level->lvl); mvprintw(0,6,num);
		sprintf(num,"%d",level->maxlvl); mvprintw(0,19,num);
		refresh(); getchar();
		}

		level->lvl = r;
		level[r].map = UpdateMap(level[r].map,Y_SCR,X_SCR);

		level[r].player->input = c; 
		
	// HANDLE INPUT:
		HandleInput(level[r].player, level[r].map, level[r].rooms);
	//////////// FOR MOVEMENT BUG ////////////////
		co = level[r].map[level[r].player->Position.yPos][level[r].player->Position.xPos];
		if(co!='@') // IF NOT STAY STILL SO THAT PREV CHAR is @:
		{ // Update the last character  
			level[r].player->Position.ch = co;
		}
	/////////////////////////////////////////////
	}

	// For returning the last created lvl save up the value for id:

	return level;
}

Level *Upstairs(Level *level, int r)
{
	if(r<=0)
	{
	mvprintw(0,0,"\n");
	mvprintw(0,0,"The staircase is blocked!! This is the first level!");
	getch(); // USE THE NCURSES GETCHAR HERE!!!
	refresh();
	level->lvl = r;
	return level; // ignore after reaching limit
	}
	
	level[r].map=UpdateMap(level[r].map,MAX_Y,MAX_X);
	r--;
	// SAVE OLD VALUES TO NEXT LEVEL!
	level[r].player->health=level[r+1].player->health;
	level[r].player->Position.type=level[r+1].player->Position.type;
	move(level[r].player->Position.yPos,level[r].player->Position.xPos);  
	level->lvl = r;
	return level;
}

Level *Downstairs(Level *level, int r)
{
	if(r>=LVLS)
	{
		mvprintw(0,0,"\n");
		mvprintw(0,0,"The staircase is blocked!! This is the last level!");
		getch(); // USE THE NCURSES GETCHAR HERE!!!
		refresh();
		level->lvl = r;
		return level; // ignore after reaching limit
	}
	if(r<level->maxlvl)
	{
		level[r].map=UpdateMap(level[r].map,MAX_Y,MAX_X);
		r++;
		// SAVE OLD VALUES TO NEXT LEVEL!
		level[r].player->health=level[r-1].player->health;
		level[r].player->Position.type=level[r-1].player->Position.type;
		level->lvl = r;
		move(level[r].player->Position.yPos,level[r].player->Position.xPos);  
		return level; // ignore after reaching limit
	}
// Make NEW level
	if(r==level->maxlvl)
	{
	level[r].map=UpdateMap(level[r].map,MAX_Y,MAX_X);
	move(level[r].player->Position.yPos,level[r].player->Position.xPos);  
	r++;
	level[r] = MakeLvl(level[r], r);
	// SAVE OLD VALUES TO NEXT LEVEL!	
	level[r].player->health=level[r-1].player->health;
	level->lvl = r;
	
	level->maxlvl++;
	return level;
	}
	else
	{
	return level;
	}
}

