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
	mvprintw(0,0,"\n");
	mvprintw(0,0,"You devour the %s corpse! (+%d hunger) You polymorph into a %s!", 
		corpse->name, corpse->nutrition, corpse->name);

	player->name = corpse->name;
	player->symbol = corpse->symbol;
	leveli->player_symbol = corpse->symbol;
	player->attack = corpse->attack;
	player->maxhp = ((corpse->maxhp)*4)+6;
	player->health = player->maxhp;

	player->hunger += corpse->nutrition;
	if(player->hunger > player->max_hunger)
	{
		player->hunger = player->max_hunger;
	}

	corpse->active = NO;
	leveli[leveli->lvl].map_backup[player->Position.yPos][player->Position.xPos] = '.';
	player->Position.ch = '.';

	PrintChar(player->Position.yPos, player->Position.xPos, leveli->player_symbol);
	move(player->Position.yPos, player->Position.xPos);

	refresh();
	getch();
	mvprintw(0,0,"\n");
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

	mvprintw(0,0,"\n");
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
		mvprintw(0,0,"\n");
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
			monster->health -= player->attack;
			sprintf(buf,"%d",player->attack);
			mvprintw(0,0,"\n");
			mvprintw(0,0,"You hit the ");
			printw(monster->name); printw(" for "); printw(buf); printw(" damage!"); 
			refresh(); getch();	
			mvprintw(0,0,"\n"); refresh();

			if(monster->health <= 0)
			{
				monster->alive = DEAD;
				sprintf(buf,"%d",monster->attack);
				mvprintw(0,0,"\n");
				mvprintw(0,0,"Congratulations you killed a ");
				printw(monster->name); printw("!  You gained "); printw(buf); printw(" XP points!");
				player->xp += monster->attack;
				refresh(); getch();
				mvprintw(0,0,"\n"); refresh();

				DropCorpse(leveli, monster, monster->Position.yPos, monster->Position.xPos);
				monster->Position.yPos = -1;
				monster->Position.xPos = -1;
				return 1;
			}

			// If the monster is following, don't fight back!
			if(monster->type!=FOLLOW)
			{
				player->health -= monster->attack;
				sprintf(buf,"%d",monster->attack);
				mvprintw(0,0,"\n");
				mvprintw(0,0,"You were hit by a ");
				printw(monster->name); printw(" for "); printw(buf); printw(" damage!"); 
				refresh(); getch();	
				mvprintw(0,0,"\n"); refresh();
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
			player->health -= monster->attack;
			sprintf(buf,"%d",monster->attack);
			mvprintw(0,0,"\n");
			mvprintw(0,0,"You were hit by a ");
			printw(monster->name); printw(" for "); printw(buf); printw(" damage!"); 
			refresh(); getch();	
			mvprintw(0,0,"\n"); refresh();
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
 
