#include "global.h"

int MAX_X; int MAX_Y; int X_SCR; int Y_SCR; int LVLS; int ROOMS;

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
	int c;
	
	level = malloc(sizeof(Level)*(LVLS+1));

	level->killer = "NO ONE";

///
	int *yx; yx = malloc(sizeof(int)*3);
//

	level->mons = 0;
	level->player_symbol = '@';
	level->turn = 0;

	// Make 1st level:
	level[0] = MakeLvl(level[0], 0, level); //level[0].lvl = 0;

	level->lvl = 0;
	level->maxlvl = 0;

	PlayerInfoBar(level);
	WelcomeTxt();

//+++++++++++++++++++++++++++++++++++++MAIN LOOOP/++++++++++++++++++++++++++++++++++++++//
	while(((c = getch()) != 'Q')) 
	{
	if(level[level->lvl].player->alive!=DEAD)
	{
		PlayerInfoBar(level);

		level[level->lvl].map = UpdateMap(level[level->lvl].map,Y_SCR,X_SCR);
	
		level[level->lvl].player->input = c; 

		if(c=='L')
		{
			ClearMsg(); mvprintw(0,0,"LVL = %d, MAXLVL = %d.", level->lvl, level->maxlvl); 		
			refresh(); getch();
			ClearMsg();
			continue;
		}
		if(c=='M')
		{
			ClearMsg(); mvprintw(0,0,"MONS = %d.", level[level->lvl].mons); 		
			refresh(); getch();
			ClearMsg();
			continue;
		}
		if(c=='C')
		{
			char chr[16];
			sprintf(chr,"%c",level->player_symbol);
			ClearMsg(); mvprintw(0,0,"CHARACTER: %s", chr); 		
			refresh(); getch();
			ClearMsg();
			continue;
		}
		
	// HANDLE INPUT:
		int action_taken = HandleInput(level,level[level->lvl], yx);

		// IMMEDIATE STAIR TRANSITIONS:
		if(c == 'r' || level[level->lvl].player->Position.type==DSTAIRS)
		{
			level = Downstairs(level,level->lvl); 
			PrintMap(MAX_Y,MAX_X,level[level->lvl].map);
			PrintChar(level[level->lvl].player->Position.yPos, level[level->lvl].player->Position.xPos, level->player_symbol);
			level[level->lvl].map = UpdateMap(level[level->lvl].map,Y_SCR,X_SCR);	
			level[level->lvl].player->Position.type = UNKNOWN;
			PlayerInfoBar(level);
			continue;
		}
		if(level[level->lvl].player->Position.type==USTAIRS)
		{
			level = Upstairs(level,level->lvl);
			PrintMap(MAX_Y,MAX_X,level[level->lvl].map);
			PrintChar(level[level->lvl].player->Position.yPos, level[level->lvl].player->Position.xPos, level->player_symbol);
			level[level->lvl].map = UpdateMap(level[level->lvl].map,Y_SCR,X_SCR);
			level[level->lvl].player->Position.type = UNKNOWN;
			PlayerInfoBar(level);
			continue;
		}

		if(!action_taken)
		{
			PlayerInfoBar(level);
			continue;
		}

		// TURN & HUNGER PROGRESSION:
		level->turn++;
		if(level[level->lvl].player->ability_cd > 0)
		{
			level[level->lvl].player->ability_cd--;
		}
		if(level[level->lvl].player->hunger_rate <= 0)
		{
			level[level->lvl].player->hunger_rate = 1;
		}
		level[level->lvl].player->hunger -= level[level->lvl].player->hunger_rate;
		if(level[level->lvl].player->hunger <= 0)
		{
			level[level->lvl].player->hunger = 0;
			level[level->lvl].player->health -= 1;
			ClearMsg();
			mvprintw(0,0,"You are starving to death!");
			PlayerInfoBar(level);
			refresh();
			if(level[level->lvl].player->health <= 0)
			{
				level[level->lvl].player->alive = DEAD;
				level->killer = "Starvation";
			}
		}

		// POISON HANDLING (PLAYER):
		if(level[level->lvl].player->alive != DEAD && level[level->lvl].player->poison_turns > 0)
		{
			level[level->lvl].player->poison_turns--;
			level[level->lvl].player->health -= 1;
			ClearMsg();
			mvprintw(0,0,"Poison wracks your body! (-1 HP, %d turns left)", level[level->lvl].player->poison_turns);
			PlayerInfoBar(level);
			refresh();
			if(level[level->lvl].player->health <= 0)
			{
				level[level->lvl].player->alive = DEAD;
				level->killer = "Lethal Poison";
			}
		}

		// POISON HANDLING (MONSTERS):
		for(int m = 0; m < level[level->lvl].mons; m++)
		{
			Entity *mon = level[level->lvl].monsters[m];
			if(mon != NULL && mon->alive == YES && mon->poison_turns > 0)
			{
				mon->poison_turns--;
				mon->health -= 1;
				if(mon->health <= 0)
				{
					mon->alive = DEAD;
					DropCorpse(level, mon, mon->Position.yPos, mon->Position.xPos);
					mon->Position.yPos = -1;
					mon->Position.xPos = -1;
				}
			}
		}

		// TROLL REGENERATION:
		if(level[level->lvl].player->alive != DEAD &&
		   level[level->lvl].player->regen_rate > 0 &&
		   (level->turn % level[level->lvl].player->regen_rate) == 0 &&
		   level[level->lvl].player->hunger > 0 &&
		   level[level->lvl].player->health < level[level->lvl].player->maxhp)
		{
			level[level->lvl].player->health += 1;
		}

		// CORPSE AGING & SPOILAGE:
		for(int c_idx = 0; c_idx < level[level->lvl].num_corpses; c_idx++)
		{
			if(level[level->lvl].corpses[c_idx].active == YES)
			{
				level[level->lvl].corpses[c_idx].age++;
				// Completely decomposes after 220 turns
				if(level[level->lvl].corpses[c_idx].age >= 220)
				{
					level[level->lvl].corpses[c_idx].active = NO;
					int cy = level[level->lvl].corpses[c_idx].Position.yPos;
					int cx = level[level->lvl].corpses[c_idx].Position.xPos;
					level[level->lvl].map_backup[cy][cx] = '.';
					if(cy != level[level->lvl].player->Position.yPos || cx != level[level->lvl].player->Position.xPos)
					{
						PrintChar(cy, cx, '.');
					}
					else
					{
						level[level->lvl].player->Position.ch = '.';
					}
				}
			}
		}

		PlayerInfoBar(level);
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
	CopyPlayer(level[r].player, level[r+1].player);
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
		CopyPlayer(level[r].player, level[r-1].player);
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
		CopyPlayer(level[r].player, level[r-1].player);
		//

		level->lvl = r;
	
		level->maxlvl++;
		return level;
	} else
	{
		return level;
	}
}

