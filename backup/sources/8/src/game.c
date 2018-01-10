#include "global.h"

int ScreenInit()
{
	MAX_X = 80; MAX_Y = 50;
	X_SCR = MAX_X+10; Y_SCR = MAX_Y+10;
	LVLS = 100;
	ROOMS = 20;

	initscr();
	noecho(); 
	refresh(); 

	return 1;
}

Level *Game(Level *level)
{
	char cp,cm = ' ';
	int c, mon0 = 0;
	
	level = malloc(sizeof(Level)*(LVLS+1));

	level->mons = 0;

	// Make 1st level:
	level[0] = MakeLvl(level[0], 0, level); //level[0].lvl = 0;

////////////////////////// BUG FIX ////////////////////////////////
	mon0 = level[0].mons;
///////////////////////////////////////////////////////////////////

	level->lvl = 0;
	level->maxlvl = 0;

	WelcomeTxt();

//+++++++++++++++++++++++++++++++++++++MAIN LOOOP/++++++++++++++++++++++++++++++++++++++//
	while((c = getchar()) != 'Q') 
	{
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
		}
		if(level[level->lvl].player->Position.type==USTAIRS)
		{
			level = Upstairs(level,level->lvl);
//			r = level->lvl; 
			PrintMap(MAX_Y,MAX_X,level[level->lvl].map);
			level[level->lvl].map = UpdateMap(level[level->lvl].map,Y_SCR,X_SCR);
		}

//		level->lvl = r;
		level[level->lvl].map = UpdateMap(level[level->lvl].map,Y_SCR,X_SCR);

		level[level->lvl].player->input = c; 
		
	// HANDLE INPUT:
		HandleInput(level[level->lvl]);

	//////////// FOR MOVEMENT BUG ////////////////
		cp = level[level->lvl].map[level[level->lvl].player->Position.yPos][level[level->lvl].player->Position.xPos];
		if(cp!='@') // IF NOT STAY STILL SO THAT PREV CHAR is @:
		{ // Update the last character  
			level[level->lvl].player->Position.ch = cp;
		}
		for(int i=0; i<level[level->lvl].mons; i++)
		{
			cm = level[level->lvl].map[level[level->lvl].monsters[i]->Position.yPos][level[level->lvl].monsters[i]->Position.xPos];
			if(cm!='@') // IF NOT STAY STILL SO THAT PREV CHAR is @:
			{ // Update the last character  
				level[level->lvl].monsters[i]->Position.ch = cm;
			}
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
	}
//+++++++++++++++++++++++++++++++++++++/MAIN LOOOP++++++++++++++++++++++++++++++++++++++//

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

////////////////////////// BUG TESTING ////////////////////////////
//	char num[16];
//	mvprintw(0,0,"\n"); mvprintw(0,0,"LVL =   , LVLI =   ."); 		
//	sprintf(num,"%d",level[r].mons); mvprintw(0,6,num);
//	sprintf(num,"%d",level->mons); mvprintw(0,17,num);
//	refresh(); getchar();
///////////////////////////////////////////////////////////////////

	// SAVE OLD VALUES TO NEXT LEVEL!
	level[r].player->health=level[r+1].player->health;
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
		r++;
		
		// SAVE OLD VALUES TO NEXT LEVEL!
		level[r].player->health=level[r-1].player->health;
		level[r].player->Position.type=level[r-1].player->Position.type;
		//

		level->lvl = r;
		move(level[r].player->Position.yPos,level[r].player->Position.xPos);  
		return level; // ignore after reaching limit
	}
// Make NEW level
	if(r==level->maxlvl)
	{
		level[r].map=UpdateMap(level[r].map,MAX_Y,MAX_X);
//		level[r] = level[r];
		move(level[r].player->Position.yPos,level[r].player->Position.xPos);  
		r++;
		level[r] = MakeLvl(level[r], r, level);
	
		// SAVE OLD VALUES TO NEXT LEVEL!	
		level[r].player->health=level[r-1].player->health;
		//

		level->lvl = r;
	
		level->maxlvl++;
		return level;
	} else
	{
		return level;
	}
}

