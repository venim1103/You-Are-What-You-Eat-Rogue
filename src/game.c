#include "global.h"

int MAX_X; int MAX_Y; int X_SCR; int Y_SCR; int LVLS; int ROOMS;
unsigned int GAME_SEED = 0;
int GAME_SEED_MODE = 0;
char GAME_SEED_LABEL[32] = "Random";

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

	UpdateFOV(&level[level->lvl]);
	RenderFOV(&level[level->lvl]);
	PlayerInfoBar(level);
	WelcomeTxt();
	UpdateFOV(&level[level->lvl]);
	RenderFOV(&level[level->lvl]);
	PlayerInfoBar(level);

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

		// IMMEDIATE STAIR & PORTAL TRANSITIONS:
		if(level[level->lvl].player->Position.type==VICTORY)
		{
			level->killer = "VICTORY";
			break;
		}
		if(level[level->lvl].player->Position.type==DSTAIRS)
		{
			level = Downstairs(level,level->lvl); 
			int nlvl = level->lvl;
			int has_miniboss = (nlvl == 3 || nlvl == 6 || nlvl == 8 || (nlvl >= 11 && (nlvl % 2 == 1)));
			PlaySound((nlvl == 10 || has_miniboss) ? "boss" : "stairs");
			level[level->lvl].map = UpdateMap(level[level->lvl].map,Y_SCR,X_SCR);	
			level[level->lvl].player->Position.type = UNKNOWN;
			UpdateFOV(&level[level->lvl]);
			RenderFOV(&level[level->lvl]);
			if(nlvl == 10)
			{
				ClearMsg();
				mvprintw(0, 0, "DANGER: Floor 10 - The Ancient Dragon's Lair! Slay the Dragon (D) to open the Golden Portal (*)!");
			}
			else if(has_miniboss)
			{
				ClearMsg();
				mvprintw(0, 0, "OMINOUS PRESENCE: A powerful Floor Mini-Boss prowls Floor %d! Defeat it for a guaranteed Relic!", nlvl);
			}
			PlayerInfoBar(level);
			refresh();
			continue;
		}
		if(level[level->lvl].player->Position.type==USTAIRS)
		{
			int prev_lvl = level->lvl;
			level = Upstairs(level,level->lvl);
			if(level->lvl != prev_lvl) PlaySound("stairs");
			level[level->lvl].map = UpdateMap(level[level->lvl].map,Y_SCR,X_SCR);
			level[level->lvl].player->Position.type = UNKNOWN;
			UpdateFOV(&level[level->lvl]);
			RenderFOV(&level[level->lvl]);
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
					int was_dragon = (mon->symbol == 'D');
					int old_xp = level[level->lvl].player->xp;
					mon->alive = DEAD;
					level[level->lvl].player->xp += mon->attack;
					CheckXPUpgrade(level[level->lvl].player, old_xp);
					DropCorpse(level, mon, mon->Position.yPos, mon->Position.xPos);
					mon->Position.yPos = -1;
					mon->Position.xPos = -1;
					ClearMsg();
					if(was_dragon)
					{
						mvprintw(0, 0, "The %s succumbs to venom! (+%d XP) A Golden Portal (*) appears!", mon->name, mon->attack);
					}
					else
					{
						mvprintw(0, 0, "The %s succumbs to lethal venom! (+%d XP)", mon->name, mon->attack);
					}
					refresh();
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

		// AMBIENT MONSTER SPAWN IN UNDISCOVERED FOG:
		if(level->turn > 0 && (level->turn % 35 == 0))
		{
			SpawnFogMonster(&level[level->lvl]);
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
					if(level[level->lvl].map_backup[cy][cx] == '%')
					{
						level[level->lvl].map_backup[cy][cx] = '.';
					}
					if(cy != level[level->lvl].player->Position.yPos || cx != level[level->lvl].player->Position.xPos)
					{
						PrintChar(cy, cx, level[level->lvl].map_backup[cy][cx]);
					}
					else
					{
						level[level->lvl].player->Position.ch = level[level->lvl].map_backup[cy][cx];
					}
				}
			}
		}

		// BACKPACK CORPSE AGING & SPOILAGE (Half-speed: 1 age tick per 2 turns):
		if((level->turn % 2) == 0)
		{
			Entity *pl = level[level->lvl].player;
			for(int inv_i = 0; inv_i < pl->num_items; inv_i++)
			{
				if(pl->inventory[inv_i].item_type == ITEM_CORPSE)
				{
					pl->inventory[inv_i].corpse_data.age++;
					if(pl->inventory[inv_i].corpse_data.age >= 220)
					{
						char *rotted_name = pl->inventory[inv_i].name;
						if(pl->equipped_weapon > inv_i) pl->equipped_weapon--;
						if(pl->equipped_armor > inv_i) pl->equipped_armor--;
						for(int k = inv_i; k < pl->num_items - 1; k++)
						{
							pl->inventory[k] = pl->inventory[k + 1];
						}
						pl->num_items--;
						inv_i--;
						ClearMsg();
						mvprintw(0, 0, "The %s corpse in your backpack rotted away into dust!", rotted_name);
						refresh();
					}
				}
			}
		}

		UpdateFOV(&level[level->lvl]);
		RenderFOV(&level[level->lvl]);

		// If the player is standing on a floor item, corpse, stairs, or portal and row 0 is empty (or has standing info), refresh it:
		{
			int py = level[level->lvl].player->Position.yPos;
			int px = level[level->lvl].player->Position.xPos;
			Item *floor_it = GetItemAt(level[level->lvl], py, px);
			Corpse *floor_corp = GetCorpseAt(level[level->lvl], py, px);
			char under_tile = level[level->lvl].map_backup[py][px];
			chtype msg_first = mvinch(0, 0) & A_CHARTEXT;
			if(msg_first == ' ' || msg_first == 0 || msg_first == 'S')
			{
				if(under_tile == '*')
				{
					ClearMsg();
					mvprintw(0, 0, "Standing on: Golden Portal (*). Press 'r' or '>' to ascend to victory!");
				}
				else if(floor_it != NULL && floor_it->active == YES)
				{
					ClearMsg();
					if(floor_it->item_type == ITEM_WEAPON)
					{
						mvprintw(0, 0, "Standing on: %s (+%d ATK weapon). Press 'g' to pick up.", floor_it->name, floor_it->bonus_att);
					}
					else if(floor_it->item_type == ITEM_ARMOR)
					{
						mvprintw(0, 0, "Standing on: %s (+%d DEF armor). Press 'g' to pick up.", floor_it->name, floor_it->bonus_def);
					}
					else if(floor_it->item_type == ITEM_CONSUMABLE)
					{
						mvprintw(0, 0, "Standing on: %s (%c) [%s]. Press 'g' to pick up.", floor_it->name, floor_it->symbol, GetConsumableDesc(floor_it->bonus_att));
					}
				}
				else if(floor_corp != NULL && floor_corp->active == YES)
				{
					ClearMsg();
					const char *st = GetCorpseStage(floor_corp->age);
					mvprintw(0, 0, "Standing on: %s %s corpse (%d nut). Press 'e' to eat or 'g' to stow.", st, floor_corp->name, floor_corp->nutrition);
				}
				else if(under_tile == '<')
				{
					ClearMsg();
					mvprintw(0, 0, "Standing on: Downstairs (<). Press '<' or 'r' to descend.");
				}
				else if(under_tile == '>')
				{
					ClearMsg();
					mvprintw(0, 0, "Standing on: Upstairs (>). Press '>' or 'r' to ascend.");
				}
				else if(under_tile == '_')
				{
					ClearMsg();
					mvprintw(0, 0, "Standing on: Metabolic Altar (_). Press 'r' to purify corpses, heal & gain +10 XP!");
				}
				else if(under_tile == '~')
				{
					ClearMsg();
					mvprintw(0, 0, "Standing on: Subterranean Spring (~). Press 'r' to drink (+450 hunger, +10 HP)!");
				}
			}
		}

		PlayerInfoBar(level);
		refresh();
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
		int old_xp = level[r].player->xp;
		level[r].player->xp += level->lvl;
		CheckXPUpgrade(level[r].player, old_xp);

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

