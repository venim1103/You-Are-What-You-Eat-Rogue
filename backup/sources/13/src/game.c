#include "global.h"

int ScreenInit()
{
	MAX_X = 160; MAX_Y = 45;
	LVLS = 100;
	ROOMS = 35;

	X_SCR = MAX_X+10; Y_SCR = MAX_Y+10;
	
	initscr();
	noecho(); 
	refresh(); 

	IntroScreen();	

	return 1;
}

Level *Game(Level *level)
{
	char cp = ' ';
	int c, mon0 = 0;
	
	level = malloc(sizeof(Level)*(LVLS+1));

	level->killer = "NO ONE";

///
	int *yx; yx = malloc(sizeof(int)*3);
//

	level->mons = 0;
	level->player_symbol = '@';

	// Make 1st level:
	level[0] = MakeLvl(level[0], 0, level); //level[0].lvl = 0;

////////////////////////// BUG FIX ////////////////////////////////
	mon0 = level[0].mons;
///////////////////////////////////////////////////////////////////

	level->lvl = 0;
	level->maxlvl = 0;

	PlayerInfoBar(level);
	WelcomeTxt();

//+++++++++++++++++++++++++++++++++++++MAIN LOOOP/++++++++++++++++++++++++++++++++++++++//
	while(((c = getchar()) != 'Q')) 
	{
	if(level[level->lvl].player->alive!=DEAD)
	{
		PlayerInfoBar(level);

////////////////////////// BUG FIX ////////////////////////////////
		level[0].mons = mon0;
///////////////////////////////////////////////////////////////////

		level[level->lvl].map = UpdateMap(level[level->lvl].map,Y_SCR,X_SCR);
	
		if(c == 'r' || level[level->lvl].player->Position.type==DSTAIRS)
		{
			level = Downstairs(level,level->lvl); 
//			r = level->lvl;
			PrintMap(MAX_Y,MAX_X,level[level->lvl].map);
			level[level->lvl].map = UpdateMap(level[level->lvl].map,Y_SCR,X_SCR);	
			level[level->lvl].player->Position.type = UNKNOWN;
			PlayerInfoBar(level);
		}
		if(level[level->lvl].player->Position.type==USTAIRS)
		{
			level = Upstairs(level,level->lvl);
//			r = level->lvl; 
			PrintMap(MAX_Y,MAX_X,level[level->lvl].map);
			level[level->lvl].map = UpdateMap(level[level->lvl].map,Y_SCR,X_SCR);
			level[level->lvl].player->Position.type = UNKNOWN;
			PlayerInfoBar(level);
		}

		
		level[level->lvl].player->input = c; 
		
	// HANDLE INPUT:
		yx = HandleInput(level,level[level->lvl], yx);
		PlayerInfoBar(level);


	//////////// FOR MOVEMENT BUG ////////////////
		cp = level[level->lvl].map[level[level->lvl].player->Position.yPos][level[level->lvl].player->Position.xPos];
		if(cp!=level->player_symbol) // IF NOT STAY STILL SO THAT PREV CHAR is @:
		{ // Update the last character  
			level[level->lvl].player->Position.ch = cp;
		}
	/////////////////////////////////////////////

		if(c=='L')
		{
		char num[16];
		mvprintw(0,0,"\n"); mvprintw(0,0,"LVL =   , MAXLVL =   ."); 		
		sprintf(num,"%d",level->lvl); mvprintw(0,6,num);
		sprintf(num,"%d",level->maxlvl); mvprintw(0,19,num);
		refresh(); getchar();
		}
		if(c=='M')
		{
		char num[16];
		mvprintw(0,0,"\n"); mvprintw(0,0,"LVL =   , LVLI =   ."); 		
		sprintf(num,"%d",level[level->lvl].mons); mvprintw(0,6,num);
		sprintf(num,"%d",level->mons); mvprintw(0,17,num);
		refresh(); getchar();
		}
		if(c=='C')
		{
		char chr[16];
		sprintf(chr,"%c",level->player_symbol);
		mvprintw(0,0,"\n"); mvprintw(0,0,"CHARACTER:"); 		
		printw(chr);
		refresh(); getchar();
		}
	} else
	{
		break;
	}

	}
//+++++++++++++++++++++++++++++++++++++/MAIN LOOOP++++++++++++++++++++++++++++++++++++++//

	// For returning the last created lvl save up the value for id:

	free(yx);

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
	level[r].map[level[r].player->Position.yPos][level[r].player->Position.xPos] = '>';

	r--;

////////////////////////// BUG TESTING ////////////////////////////
//	char num[16];
//	mvprintw(0,0,"\n"); mvprintw(0,0,"LVL =   , LVLI =   ."); 		
//	sprintf(num,"%d",level[r].mons); mvprintw(0,6,num);
//	sprintf(num,"%d",level->mons); mvprintw(0,17,num);
//	refresh(); getchar();
///////////////////////////////////////////////////////////////////

	// SAVE OLD VALUES TO NEXT LEVEL!
	level[r].player->health=level[r+1].player->health;
	level[r].player->attack=level[r+1].player->attack;
	level[r].player->xp=level[r+1].player->xp;
	level[r].player->name=level[r+1].player->name;
	level[r].player->Position.type=level[r+1].player->Position.type;
	//

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
		level[r].map[level[r].player->Position.yPos][level[r].player->Position.xPos] = '<';
		r++;
		
		// SAVE OLD VALUES TO NEXT LEVEL!
		level[r].player->health=level[r-1].player->health;
		level[r].player->Position.type=level[r-1].player->Position.type;
		level[r].player->attack=level[r-1].player->attack;
		level[r].player->xp=level[r-1].player->xp;
		level[r].player->name=level[r-1].player->name;
		//

		level->lvl = r;
		move(level[r].player->Position.yPos,level[r].player->Position.xPos);  
		return level; // ignore after reaching limit
	}
// Make NEW level
	if(r==level->maxlvl)
	{
		// WHEN ENTERING A NEW LVL GAIN XP!
		level[r].player->xp += level->lvl;

		level[r].map=UpdateMap(level[r].map,MAX_Y,MAX_X);
		level[r].map[level[r].player->Position.yPos][level[r].player->Position.xPos] = '<';
		move(level[r].player->Position.yPos,level[r].player->Position.xPos);  
		r++;
		level[r] = MakeLvl(level[r], r, level);
	
		// SAVE OLD VALUES TO NEXT LEVEL!	
		level[r].player->health=level[r-1].player->health;
		level[r].player->attack=level[r-1].player->attack;
		level[r].player->xp=level[r-1].player->xp;
		level[r].player->name=level[r-1].player->name;
		//

		level->lvl = r;
	
		level->maxlvl++;
		return level;
	} else
	{
		return level;
	}
}

