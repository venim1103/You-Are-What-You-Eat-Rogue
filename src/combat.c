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
			case 'g': leveli[lvl].corpses[idx].nutrition = 400; break;
			case 'T': leveli[lvl].corpses[idx].nutrition = 700; break;
			default:  leveli[lvl].corpses[idx].nutrition = 300; break;
		}
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

int EatCorpse(Level *leveli, Level level, Entity *player, Corpse *corpse)
{
	int nut = corpse->nutrition;
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

	ClearMsg();
	mvprintw(0,0,"Devoured %s! (+%d hunger) Polymorphed into %s! [%s]", 
		corpse->name, nut, corpse->name, player->trait_desc);

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

	ClearMsg();
	mvprintw(0,0,"You see a %s corpse here. Eat it? (y/n)", corpse->name);
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
		mvprintw(0,0,"You decide not to eat the %s corpse.", corpse->name);
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

			// Snake Venomous Strike trait
			if(player->poison_attack && monster->symbol != 's' && monster->health > 0)
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

				if(monster->poison_attack && player->symbol != 's' && player->health > 0)
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
	
	if(attacker==MONSTER)
	{
		if(monster == NULL || monster->alive != YES)
		{
			return 0;
		}

		if(monster->health > 0)
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

			if(monster->poison_attack && player->symbol != 's' && player->health > 0)
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

int Eat(Level *leveli, Level level, Entity *player, Entity *monster)
{
	if(monster != NULL)
	{
		Corpse dummy;
		dummy.Position = monster->Position;
		dummy.symbol = monster->symbol;
		dummy.name = monster->name;
		dummy.maxhp = monster->maxhp;
		dummy.attack = monster->attack;
		dummy.nutrition = 400;
		dummy.active = YES;
		return EatCorpse(leveli, level, player, &dummy);
	}
	return 0;
}
 
