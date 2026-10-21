#include "global.h"

int DropCorpse(Level *leveli, Entity *monster, int y, int x)
{
	int lvl = leveli->lvl;
	int idx = leveli[lvl].num_corpses;
	if(idx < leveli[lvl].max_corpses)
	{
		leveli[lvl].corpses[idx].Position.yPos = y;
		leveli[lvl].corpses[idx].Position.xPos = x;
		leveli[lvl].corpses[idx].symbol = monster->symbol;
		leveli[lvl].corpses[idx].name = monster->name;
		leveli[lvl].corpses[idx].maxhp = monster->maxhp;
		leveli[lvl].corpses[idx].attack = monster->attack;
		leveli[lvl].corpses[idx].active = YES;

		switch(monster->symbol)
		{
			case 'x': leveli[lvl].corpses[idx].nutrition = 150; break;
			case 's': leveli[lvl].corpses[idx].nutrition = 250; break;
			case 'V': leveli[lvl].corpses[idx].nutrition = 100; break;
			case 'B': leveli[lvl].corpses[idx].nutrition = 320; break;
			case 'G': leveli[lvl].corpses[idx].nutrition = 120; break;
			case 'g': leveli[lvl].corpses[idx].nutrition = 400; break;
			case 'T': leveli[lvl].corpses[idx].nutrition = 700; break;
			default:  leveli[lvl].corpses[idx].nutrition = 300; break;
		}
		leveli[lvl].corpses[idx].age = 0;
		leveli[lvl].num_corpses++;
	}

	leveli[lvl].map_backup[y][x] = '%';
	PrintChar(y, x, '%');
	return 1;
}

Corpse *GetCorpseAt(Level level, int y, int x)
{
	for(int i = 0; i < level.num_corpses; i++)
	{
		if(level.corpses[i].active == YES &&
		   level.corpses[i].Position.yPos == y &&
		   level.corpses[i].Position.xPos == x)
		{
			return &level.corpses[i];
		}
	}
	return NULL;
}

const char *GetCorpseStage(int age)
{
	if(age < 40) return "Fresh";
	if(age < 90) return "Aging";
	if(age < 150) return "Tainted";
	return "Rotten";
}

int EatCorpse(Level *leveli, Level level, Entity *player, Corpse *corpse)
{
	int nut = corpse->nutrition;
	const char *stage = GetCorpseStage(corpse->age);
	int sick = 0;

	// Nutrition and spoilage degradation:
	if(strcmp(stage, "Aging") == 0)
	{
		nut = nut * 80 / 100;
	}
	else if(strcmp(stage, "Tainted") == 0)
	{
		nut = nut * 50 / 100;
		// 50% chance of food poisoning unless player has scavenger bonus or acid/poison immunity
		if(player->scavenger_bonus == 0 && player->symbol != 'B' && player->symbol != 's')
		{
			if((rand() % 100) < 50)
			{
				sick = 1;
			}
		}
	}
	else if(strcmp(stage, "Rotten") == 0)
	{
		nut = nut * 25 / 100;
		// 85% chance of severe food poisoning / sickness
		if(player->scavenger_bonus == 0 && player->symbol != 'B')
		{
			if((rand() % 100) < 85)
			{
				sick = 2;
			}
		}
	}

	if(player->scavenger_bonus > 0)
	{
		nut = nut + (nut * player->scavenger_bonus / 100);
	}

	player->name = corpse->name;
	player->symbol = corpse->symbol;
	leveli->player_symbol = corpse->symbol;
	player->attack = corpse->attack;
	player->maxhp = ((corpse->maxhp)*4)+6;
	player->health = player->maxhp;

	player->hunger += nut;
	if(player->hunger > player->max_hunger)
	{
		player->hunger = player->max_hunger;
	}

	ApplyFormTraits(player, corpse->symbol);

	// Handle Sickness/Poison penalties from spoiled meat
	if(sick == 1)
	{
		player->poison_turns += 4;
		player->health -= 3;
		if(player->health < 1) player->health = 1;
	}
	else if(sick == 2)
	{
		player->poison_turns += 8;
		player->health -= 6;
		if(player->health < 1) player->health = 1;
	}

	ClearMsg();
	if(sick > 0)
	{
		mvprintw(0,0,"Ugh! You ate a %s %s corpse! SICKENED! (+%d hunger) Polymorphed into %s!", 
			stage, corpse->name, nut, corpse->name);
	}
	else
	{
		mvprintw(0,0,"Devoured %s %s! (+%d hunger) Polymorphed into %s! [%s]", 
			stage, corpse->name, nut, corpse->name, player->trait_desc);
	}

	corpse->active = NO;
	leveli[leveli->lvl].map_backup[player->Position.yPos][player->Position.xPos] = '.';
	player->Position.ch = '.';

	PrintChar(player->Position.yPos, player->Position.xPos, leveli->player_symbol);
	move(player->Position.yPos, player->Position.xPos);

	refresh();
	getch();
	ClearMsg();
	refresh();
	return 1;
}

int AskEatCorpse(Level *leveli, Level level, Entity *player, int y, int x)
{
	Corpse *corpse = GetCorpseAt(leveli[leveli->lvl], y, x);
	if(corpse == NULL || corpse->active != YES)
	{
		return 0;
	}

	const char *stage = GetCorpseStage(corpse->age);
	ClearMsg();
	mvprintw(0,0,"You see a %s %s corpse here. Eat it? (y/n)", stage, corpse->name);
	refresh();

	int ch = getch();
	if(ch == 'y' || ch == 'Y')
	{
		EatCorpse(leveli, level, player, corpse);
	}
	else
	{
		player->Position.ch = '%';
		ClearMsg();
		mvprintw(0,0,"You decide not to eat the %s %s corpse.", stage, corpse->name);
		refresh();
	}
	return 1;
}

int Combat(Level *leveli, Level level, Entity *player, Entity *monster, int attacker)
{
	char buf[8];
	
	if(attacker==PLAYER)
	{
		if(monster == NULL || monster->alive != YES)
		{
			return 0;
		}

		move(player->Position.yPos,player->Position.xPos);

		if(monster->health > 0)
		{
			// Check monster evasion (e.g. Bat, Ghost)
			if(monster->evasion_chance > 0 && (rand() % 100) < monster->evasion_chance)
			{
				ClearMsg();
				mvprintw(0,0,"The %s deftly evades your attack!", monster->name);
				refresh(); getch();
				ClearMsg(); refresh();
			}
			else
			{
				int dmg = player->attack;

				// Cleave trait
				if(player->cleave_chance > 0 && (rand() % 100) < player->cleave_chance)
				{
					dmg += 3;
					ClearMsg();
					mvprintw(0,0,"CRUSHING CLEAVE! (+3 damage)");
					refresh(); getch();
				}

				monster->health -= dmg;
				sprintf(buf,"%d",dmg);
				ClearMsg();
				mvprintw(0,0,"You hit the ");
				printw(monster->name); printw(" for "); printw(buf); printw(" damage!"); 
				refresh(); getch();	
				ClearMsg(); refresh();

				// Bat Life Drain trait
				if(player->life_drain > 0 && player->health < player->maxhp)
				{
					player->health += 1;
					ClearMsg();
					mvprintw(0,0,"You drain life from the %s! (+1 HP)", monster->name);
					refresh(); getch();
					ClearMsg(); refresh();
				}

				// Slime Acid Corrosion retaliation on hit
				if(monster->acid_corrosion > 0)
				{
					player->health -= 1;
					ClearMsg();
					mvprintw(0,0,"Acidic spray burns you! (-1 HP)");
					refresh(); getch();
					ClearMsg(); refresh();
					if(player->health <= 0)
					{
						player->alive = DEAD;
						leveli->killer = "Caustic Slime Acid";
					}
				}

				// Snake Venomous Strike trait
				if(player->poison_attack && monster->symbol != 's' && monster->symbol != 'B' && monster->health > 0)
				{
					monster->poison_turns = 3;
					ClearMsg();
					mvprintw(0,0,"You inject venom into the %s!", monster->name);
					refresh(); getch();
					ClearMsg(); refresh();
				}

				// Spider Skitter trait: second attack
				if(player->extra_attack_chance > 0 && (rand() % 100) < player->extra_attack_chance && monster->health > 0)
				{
					ClearMsg();
					mvprintw(0,0,"SKITTER! You strike a second time!");
					refresh(); getch();
					monster->health -= player->attack;
					sprintf(buf,"%d",player->attack);
					ClearMsg();
					mvprintw(0,0,"Second bite hits for "); printw(buf); printw(" damage!");
					refresh(); getch();
					ClearMsg(); refresh();
				}
			}

			if(monster->health <= 0)
			{
				monster->alive = DEAD;
				sprintf(buf,"%d",monster->attack);
				ClearMsg();
				mvprintw(0,0,"Congratulations you killed a ");
				printw(monster->name); printw("!  You gained "); printw(buf); printw(" XP points!");
				player->xp += monster->attack;
				refresh(); getch();
				ClearMsg(); refresh();

				DropCorpse(leveli, monster, monster->Position.yPos, monster->Position.xPos);
				monster->Position.yPos = -1;
				monster->Position.xPos = -1;
				return 1;
			}

			// If the monster is following, don't fight back!
			if(monster->type!=FOLLOW)
			{
				// Check player evasion (Bat, Ghost)
				if(player->evasion_chance > 0 && (rand() % 100) < player->evasion_chance)
				{
					ClearMsg();
					mvprintw(0,0,"You gracefully evade the %s's attack!", monster->name);
					refresh(); getch();
					ClearMsg(); refresh();
				}
				else
				{
					int m_dmg = monster->attack;
					if(monster->cleave_chance > 0 && (rand() % 100) < monster->cleave_chance)
					{
						m_dmg += 3;
						ClearMsg();
						mvprintw(0,0,"The %s lands a CRUSHING CLEAVE!", monster->name);
						refresh(); getch();
					}

					player->health -= m_dmg;
					sprintf(buf,"%d",m_dmg);
					ClearMsg();
					mvprintw(0,0,"You were hit by a ");
					printw(monster->name); printw(" for "); printw(buf); printw(" damage!"); 
					refresh(); getch();	
					ClearMsg(); refresh();

					// Player acid reflection if Slime
					if(player->acid_corrosion > 0)
					{
						monster->health -= 1;
						ClearMsg();
						mvprintw(0,0,"Your caustic body corrodes the %s! (-1 HP)", monster->name);
						refresh(); getch();
						ClearMsg(); refresh();
					}

					// Bat monster life drain
					if(monster->life_drain > 0 && monster->health < monster->maxhp)
					{
						monster->health += 1;
					}

					if(monster->poison_attack && player->symbol != 's' && player->symbol != 'B' && player->health > 0)
					{
						player->poison_turns = 3;
						ClearMsg();
						mvprintw(0,0,"The %s bit you with lethal venom!", monster->name);
						refresh(); getch();
						ClearMsg(); refresh();
					}
				}
			}
		}
	}
	
	if(attacker==MONSTER)
	{
		if(monster == NULL || monster->alive != YES)
		{
			return 0;
		}

		if(monster->health > 0)
		{
			// Check player evasion (Bat, Ghost)
			if(player->evasion_chance > 0 && (rand() % 100) < player->evasion_chance)
			{
				ClearMsg();
				mvprintw(0,0,"You gracefully evade the %s's attack!", monster->name);
				refresh(); getch();
				ClearMsg(); refresh();
			}
			else
			{
				int m_dmg = monster->attack;
				if(monster->cleave_chance > 0 && (rand() % 100) < monster->cleave_chance)
				{
					m_dmg += 3;
					ClearMsg();
					mvprintw(0,0,"The %s lands a CRUSHING CLEAVE!", monster->name);
					refresh(); getch();
				}

				player->health -= m_dmg;
				sprintf(buf,"%d",m_dmg);
				ClearMsg();
				mvprintw(0,0,"You were hit by a ");
				printw(monster->name); printw(" for "); printw(buf); printw(" damage!"); 
				refresh(); getch();	
				ClearMsg(); refresh();

				// Player acid reflection if Slime
				if(player->acid_corrosion > 0)
				{
					monster->health -= 1;
					ClearMsg();
					mvprintw(0,0,"Your caustic body corrodes the %s! (-1 HP)", monster->name);
					refresh(); getch();
					ClearMsg(); refresh();
				}

				// Bat monster life drain
				if(monster->life_drain > 0 && monster->health < monster->maxhp)
				{
					monster->health += 1;
				}

				if(monster->poison_attack && player->symbol != 's' && player->symbol != 'B' && player->health > 0)
				{
					player->poison_turns = 3;
					ClearMsg();
					mvprintw(0,0,"The %s bit you with lethal venom!", monster->name);
					refresh(); getch();
					ClearMsg(); refresh();
				}

				if(monster->extra_attack_chance > 0 && (rand() % 100) < monster->extra_attack_chance && player->health > 0)
				{
					ClearMsg();
					mvprintw(0,0,"The %s skitters rapidly and bites again!", monster->name);
					refresh(); getch();
					player->health -= monster->attack;
					sprintf(buf,"%d",monster->attack);
					ClearMsg();
					mvprintw(0,0,"Second strike hits for "); printw(buf); printw(" damage!");
					refresh(); getch();
					ClearMsg(); refresh();
				}
			}
		}
	}

	if(player->health<=0)
	{
		player->alive=DEAD;
		if(monster != NULL)
		{
			leveli->killer = monster->name;
		}
	}

	return 1;
}

int ActivateFormAbility(Level *leveli, Level level, Entity *player)
{
	if(player == NULL || player->alive != YES)
	{
		return 0;
	}

	if(player->ability_cd > 0)
	{
		ClearMsg();
		mvprintw(0,0,"Ability '%s' is on cooldown! (%d turns left)", player->ability_name, player->ability_cd);
		refresh();
		return 0;
	}

	if(player->hunger < player->ability_cost)
	{
		ClearMsg();
		mvprintw(0,0,"Too hungry to use '%s'! (Needs %d hunger)", player->ability_name, player->ability_cost);
		refresh();
		return 0;
	}

	int py = player->Position.yPos;
	int px = player->Position.xPos;
	int used = 0;

	switch(player->symbol)
	{
		case '@': // Human: Bandage
			ClearMsg();
			player->poison_turns = 0;
			player->health += 5;
			if(player->health > player->maxhp) player->health = player->maxhp;
			mvprintw(0,0,"You bandage your wounds! (+5 HP, Cured Poison)");
			used = 1;
			break;

		case 'x': // Spider: Spin Web
		{
			ClearMsg();
			int dy[4] = {-1, 1, 0, 0};
			int dx[4] = {0, 0, -1, 1};
			int hits = 0;
			for(int i = 0; i < 4; i++)
			{
				Entity *mon = GetMonsterAt(py + dy[i], px + dx[i], level);
				if(mon != NULL && mon->alive == YES)
				{
					mon->poison_turns += 2;
					hits++;
				}
			}
			if(hits > 0)
			{
				mvprintw(0,0,"You shoot sticky webbing! Ensnared %d nearby enemies!", hits);
			}
			else
			{
				mvprintw(0,0,"You spin webs around you, setting a defensive perimeter!");
			}
			used = 1;
			break;
		}

		case 's': // Snake: Venom Spit
		{
			ClearMsg();
			// Look for closest monster within 4 tiles
			Entity *target = NULL;
			int min_dist = 999;
			for(int m = 0; m < level.mons; m++)
			{
				Entity *mon = level.monsters[m];
				if(mon != NULL && mon->alive == YES)
				{
					int dist = abs(mon->Position.yPos - py) + abs(mon->Position.xPos - px);
					if(dist <= 4 && dist < min_dist)
					{
						min_dist = dist;
						target = mon;
					}
				}
			}
			if(target != NULL)
			{
				target->health -= (player->attack + 2);
				target->poison_turns += 4;
				mvprintw(0,0,"You spit toxic venom at %s! (%d damage, 4t poison)", target->name, player->attack + 2);
				if(target->health <= 0)
				{
					target->alive = DEAD;
					DropCorpse(leveli, target, target->Position.yPos, target->Position.xPos);
					target->Position.yPos = -1;
					target->Position.xPos = -1;
				}
			}
			else
			{
				mvprintw(0,0,"You spit venom forward into the shadows!");
			}
			used = 1;
			break;
		}

		case 'g': // Goblin: Flashbang
		{
			ClearMsg();
			int dy[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
			int dx[8] = {0, 0, -1, 1, 1, -1, 1, -1};
			int blinded = 0;
			for(int i = 0; i < 8; i++)
			{
				Entity *mon = GetMonsterAt(py + dy[i], px + dx[i], level);
				if(mon != NULL && mon->alive == YES)
				{
					mon->health -= 2;
					blinded++;
					if(mon->health <= 0)
					{
						mon->alive = DEAD;
						DropCorpse(leveli, mon, mon->Position.yPos, mon->Position.xPos);
						mon->Position.yPos = -1;
						mon->Position.xPos = -1;
					}
				}
			}
			mvprintw(0,0,"BANG! Tinkered flashbang detonates! Blinded %d nearby foes!", blinded);
			used = 1;
			break;
		}

		case 'T': // Troll: Ground Slam
		{
			ClearMsg();
			int dy[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
			int dx[8] = {0, 0, -1, 1, 1, -1, 1, -1};
			int hit_count = 0;
			for(int i = 0; i < 8; i++)
			{
				Entity *mon = GetMonsterAt(py + dy[i], px + dx[i], level);
				if(mon != NULL && mon->alive == YES)
				{
					mon->health -= 7;
					hit_count++;
					if(mon->health <= 0)
					{
						mon->alive = DEAD;
						DropCorpse(leveli, mon, mon->Position.yPos, mon->Position.xPos);
						mon->Position.yPos = -1;
						mon->Position.xPos = -1;
					}
				}
			}
			mvprintw(0,0,"ROAR! GROUND SLAM! Tremor crushes %d surrounding enemies for 7 damage!", hit_count);
			used = 1;
			break;
		}

		case 'G': // Ghost: Shadow Blink
		{
			ClearMsg();
			int ran_dir = rand() % 4;
			int dy[4] = {-3, 3, 0, 0};
			int dx[4] = {0, 0, -3, 3};
			int ny = py + dy[ran_dir];
			int nx = px + dx[ran_dir];
			if(ny > 0 && ny < MAX_Y && nx > 0 && nx < MAX_X)
			{
				PrintChar(py, px, level.map_backup[py][px]);
				player->Position.yPos = ny;
				player->Position.xPos = nx;
				player->Position.ch = (mvinch(ny, nx) & A_CHARTEXT);
				PrintChar(ny, nx, leveli->player_symbol);
				move(ny, nx);
				mvprintw(0,0,"You ethereal-warp across space through the shadows!");
				used = 1;
			}
			else
			{
				mvprintw(0,0,"Ethereal warp dissipated against the void barrier.");
				used = 1;
			}
			break;
		}

		case 'B': // Slime: Acid Jet
		{
			ClearMsg();
			int dy[4] = {-1, 1, 0, 0};
			int dx[4] = {0, 0, -1, 1};
			int dissolved = 0;
			for(int step = 1; step <= 3; step++)
			{
				for(int d = 0; d < 4; d++)
				{
					int ny = py + dy[d] * step;
					int nx = px + dx[d] * step;
					if(ny > 0 && ny < MAX_Y && nx > 0 && nx < MAX_X)
					{
						Entity *mon = GetMonsterAt(ny, nx, level);
						if(mon != NULL && mon->alive == YES)
						{
							mon->health -= 5;
							dissolved++;
							if(mon->health <= 0)
							{
								mon->alive = DEAD;
								DropCorpse(leveli, mon, mon->Position.yPos, mon->Position.xPos);
								mon->Position.yPos = -1;
								mon->Position.xPos = -1;
							}
						}
					}
				}
			}
			mvprintw(0,0,"SPLAT! Acid Jet dissolves %d targets with burning caustic spray!", dissolved);
			used = 1;
			break;
		}

		case 'V': // Vampire Bat: Echo Screech
		{
			ClearMsg();
			for(int m = 0; m < level.mons; m++)
			{
				Entity *mon = level.monsters[m];
				if(mon != NULL && mon->alive == YES)
				{
					PrintChar(mon->Position.yPos, mon->Position.xPos, mon->symbol);
				}
			}
			mvprintw(0,0,"SKREEE! Echolocation pulse detects all creature locations on this floor!");
			used = 1;
			break;
		}

		default:
			break;
	}

	if(used)
	{
		player->hunger -= player->ability_cost;
		if(player->hunger < 0) player->hunger = 0;
		player->ability_cd = player->ability_max_cd;
		refresh();
		getch();
		ClearMsg();
		refresh();
		return 1;
	}

	return 0;
}
 
