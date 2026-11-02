#include "global.h"

Entity **AddMonsters(Level *level, Entity rooms, int lvl)
{
	int makeamount = 0;
	makeamount = ((ROOMS/2)+lvl);
	if(makeamount > 45) makeamount = 45;

	Entity **monsters;
	level->max_mons = 64;
	monsters = malloc(sizeof(Entity*)*level->max_mons);
	int *yx; yx=malloc(sizeof(int)*3); 

	level->mons = 0;

	int k = 0;

	// Floor 10 Boss Encounter: Ancient Dragon + Elite Bodyguards (2 Trolls + 1 Necromancer Lich)
	if(lvl == 10)
	{
		monsters[k] = CreateMon('D', 45, 8, DRAGON, FOLLOW, "Ancient Dragon");
		monsters[k]->is_elite = 2;
		yx = MonSetup(monsters[k], rooms, yx);
		k++;

		for(int b = 0; b < 2 && k < level->max_mons; b++)
		{
			monsters[k] = CreateMon('T', 15, 5, TROLL, FOLLOW, "Troll");
			yx = MonSetup(monsters[k], rooms, yx);
			k++;
		}
		if(k < level->max_mons)
		{
			monsters[k] = CreateMon('L', 12, 6, LICH, FOLLOW, "Necromancer Lich");
			yx = MonSetup(monsters[k], rooms, yx);
			k++;
		}
	}
	else if(lvl == 3 && k < level->max_mons)
	{
		monsters[k] = CreateMon('x', 16, 4, SPIDER, FOLLOW, "Broodmother Arachne");
		monsters[k]->is_elite = 2;
		monsters[k]->poison_attack = 1;
		monsters[k]->extra_attack_chance = 50;
		monsters[k]->trait_desc = "Mini-Boss: Venomous Broodmother (50% Double-Bite, Poison, Drops Relic)";
		yx = MonSetup(monsters[k], rooms, yx);
		k++;
	}
	else if(lvl == 6 && k < level->max_mons)
	{
		monsters[k] = CreateMon('g', 24, 6, GOBLIN, FOLLOW, "Goblin Warlord");
		monsters[k]->is_elite = 2;
		monsters[k]->cleave_chance = 35;
		monsters[k]->trait_desc = "Mini-Boss: Warlord Chieftain (35% Cleave +3 dmg, Drops Relic)";
		yx = MonSetup(monsters[k], rooms, yx);
		k++;
	}
	else if(lvl == 8 && k < level->max_mons)
	{
		monsters[k] = CreateMon('L', 28, 8, LICH, FOLLOW, "Arch-Lich Malakor");
		monsters[k]->is_elite = 2;
		monsters[k]->life_drain = 3;
		monsters[k]->evasion_chance = 25;
		monsters[k]->trait_desc = "Mini-Boss: Undead Arch-Mage (+3 HP Drain, 25% Evasion, Drops Relic)";
		yx = MonSetup(monsters[k], rooms, yx);
		k++;
	}
	else if(lvl >= 11 && (lvl % 2 == 1) && k < level->max_mons)
	{
		monsters[k] = CreateMon('T', 36 + (lvl - 11) * 2, 9 + (lvl - 11) / 2, TROLL, FOLLOW, "Elder Behemoth");
		monsters[k]->is_elite = 2;
		monsters[k]->cleave_chance = 50;
		monsters[k]->trait_desc = "Mini-Boss: Abyssal Behemoth (50% Cleave, Regen, Drops Relic)";
		yx = MonSetup(monsters[k], rooms, yx);
		k++;
	}

	// Count up to all the rooms
	for(int i=0; i<makeamount && k < level->max_mons; i++)
	{
		if((rand()%2) == YES) // If every 50% say YES!
		{
			monsters[k] = SelMonster(lvl);
			if(lvl >= 1)
			{
				int elite_chance = 10 + (lvl * 2);
				if(elite_chance > 32) elite_chance = 32;
				if((rand() % 100) < elite_chance)
				{
					PromoteToElite(monsters[k], lvl);
				}
			}
			yx = MonSetup(monsters[k], rooms, yx);
			k++;
		}
	}	

	level->mons = k;

	free(yx);
	
	return monsters;
}

int SpawnFogMonster(Level *level)
{
	if(level == NULL || level->monsters == NULL || level->mons >= level->max_mons)
	{
		return 0;
	}

	int py = level->player->Position.yPos;
	int px = level->player->Position.xPos;
	int *yx = malloc(sizeof(int) * 3);
	int spawned = 0;

	for(int attempt = 0; attempt < 60; attempt++)
	{
		if(level->rooms.mode <= 0) break;
		int rm = rand() % level->rooms.mode;
		yx = RandomCoords(yx, level->rooms.mode, level->rooms.Area[rm]);
		int sy = yx[0];
		int sx = yx[1];

		if(sy < 1 || sy >= MAX_Y - 1 || sx < 1 || sx >= MAX_X - 1)
		{
			continue;
		}

		// Must be in undiscovered fog
		if(level->discovered != NULL && level->discovered[sy][sx] != 0)
		{
			continue;
		}
		if(level->visible != NULL && level->visible[sy][sx] != 0)
		{
			continue;
		}

		// Must be walkable floor
		if(level->map_backup[sy][sx] != '.')
		{
			continue;
		}

		// Distance check
		if(abs(sy - py) + abs(sx - px) < 12)
		{
			continue;
		}

		// Entity collision check
		if(GetMonsterAt(sy, sx, *level) != NULL)
		{
			continue;
		}

		// Spawn monster according to level difficulty
		Entity *mon = SelMonster(level->lvl);
		if(level->lvl >= 2 && (rand() % 100) < 20)
		{
			PromoteToElite(mon, level->lvl);
		}
		mon->Position.yPos = sy;
		mon->Position.xPos = sx;
		mon->Position.ch = '.';
		mon->Position.c = '.';
		mon->alive = YES;

		level->monsters[level->mons] = mon;
		level->mons++;
		spawned = 1;

		ClearMsg();
		mvprintw(0, 0, "You hear menacing scuffling echoing in the dark depths...");
		refresh();
		break;
	}

	free(yx);
	return spawned;
}

Entity *SelMonster(int lvl)
{
	int mon = SPIDER;

	if(lvl <= 2)
	{
		// Floors 0-2: Spider, Snake, Bat
		int r = rand() % 3;
		if(r == 0) mon = SPIDER;
		else if(r == 1) mon = SNAKE;
		else mon = BAT;
	}
	else if(lvl <= 6)
	{
		// Floors 3-6: Bat, Snake, Goblin, Slime (and Lich starting on Floor 5+)
		if(lvl >= 5 && (rand() % 5) == 0)
		{
			mon = LICH;
		}
		else
		{
			int r = rand() % 4;
			if(r == 0) mon = BAT;
			else if(r == 1) mon = SNAKE;
			else if(r == 2) mon = GOBLIN;
			else mon = SLIME;
		}
	}
	else if(lvl <= 12)
	{
		// Floors 7-12: Goblin, Slime, Ghost, Troll, Necromancer Lich
		int r = rand() % 5;
		if(r == 0) mon = GOBLIN;
		else if(r == 1) mon = SLIME;
		else if(r == 2) mon = GHOST;
		else if(r == 3) mon = TROLL;
		else mon = LICH;
	}
	else
	{
		// Floors 13+: High danger (Troll, Lich, Ghost, Slime, occasional Bat/Goblin)
		int roll = rand() % 100;
		if(roll < 35) mon = TROLL;
		else if(roll < 55) mon = LICH;
		else if(roll < 75) mon = GHOST;
		else if(roll < 90) mon = SLIME;
		else if(roll < 95) mon = GOBLIN;
		else mon = BAT;
	}

	switch(mon)
	{
		case SPIDER:
			return CreateMon('x', 2, 1, SPIDER, RANDOM_X, "Spider");
		break;	
		
		case SNAKE:
			return CreateMon('s', 3, 2, SNAKE, RANDOM_S, "Snake");
		break;	

		case BAT:
			return CreateMon('V', 2, 2, BAT, RANDOM_X, "Vampire Bat");
		break;

		case GOBLIN:
			return CreateMon('g', 5, 3, GOBLIN, FOLLOW, "Goblin");
		break;	

		case SLIME:
			return CreateMon('B', 8, 2, SLIME, RANDOM_S, "Acid Slime");
		break;

		case GHOST:
			return CreateMon('G', 4, 3, GHOST, FOLLOW, "Ghost");
		break;
		
		case TROLL:
			return CreateMon('T', 15, 5, TROLL, FOLLOW, "Troll");
		break;

		case LICH:
			return CreateMon('L', 12, 6, LICH, FOLLOW, "Necromancer Lich");
		break;

		case DRAGON:
			return CreateMon('D', 45, 8, DRAGON, FOLLOW, "Ancient Dragon");
		break;

		default:
			return CreateMon('g', 5, 3, GOBLIN, FOLLOW, "Goblin");
		break;	
	}	
	return CreateMon('T', 15, 5, TROLL, FOLLOW, "Troll");
}

Entity *CreateMon(char c, int hp, int att, int mode, int type, char *name)
{
	Entity *monster;
	monster = malloc(sizeof(Entity));

	monster->symbol = c;
	monster->health = hp;
	monster->attack = att;
	monster->maxhp = hp;
	monster->mode = mode;
	monster->type = type;
	monster->name = name;

	monster->Position.type = NO;
	monster->alive = YES;
	monster->poison_turns = 0;
	monster->snare_turns = 0;
	monster->is_elite = 0;
	ApplyFormTraits(monster, c);

	return monster;
}

Entity *PromoteToElite(Entity *mon, int lvl)
{
	if(mon == NULL || mon->symbol == 'D') return mon;
	mon->is_elite = 1;
	mon->type = FOLLOW;
	mon->maxhp = (mon->maxhp * 3) / 2 + 3 + (lvl / 3);
	mon->health = mon->maxhp;
	mon->attack += 2;

	switch(mon->symbol)
	{
		case 'x':
			mon->name = "Elite Venomweaver";
			mon->poison_attack = 1;
			mon->trait_desc = "Elite Arachnid (Skitter 40%, Venomous Bite, Aggressive)";
			break;
		case 's':
			mon->name = "Elite Pit Viper";
			mon->evasion_chance = 15;
			mon->trait_desc = "Elite Serpent (Venomous Strike, 15% Evasion, Aggressive)";
			break;
		case 'V':
			mon->name = "Elite Bloodwing Bat";
			mon->life_drain = 2;
			mon->trait_desc = "Elite Flyer (50% Evasion, +2 HP Blood Drain, Aggressive)";
			break;
		case 'g':
			mon->name = "Elite Goblin Raider";
			mon->cleave_chance = 20;
			mon->trait_desc = "Elite Raider (20% Cleave, Scavenger +50%, Aggressive)";
			break;
		case 'B':
			mon->name = "Elite Caustic Ooze";
			mon->trait_desc = "Elite Ooze (Caustic Acid Shield, High HP, Aggressive)";
			break;
		case 'G':
			mon->name = "Elite Phantom";
			mon->evasion_chance = 35;
			mon->trait_desc = "Elite Wraith (Phases Walls, 35% Evasion, Aggressive)";
			break;
		case 'T':
			mon->name = "Elite War Troll";
			mon->cleave_chance = 45;
			mon->trait_desc = "Elite Juggernaut (45% Cleave +3 dmg, Regen, Aggressive)";
			break;
		case 'L':
			mon->name = "Elite Lich Acolyte";
			mon->life_drain = 3;
			mon->trait_desc = "Elite Undead (+3 HP Soul Drain, 15% Evasion, Aggressive)";
			break;
		default:
			break;
	}
	return mon;
}

int *MonSetup(Entity *monster, Entity room, int *yx)
{
	int safe_rooms = (room.mode > 0) ? room.mode : 1;
	int random = (rand() % safe_rooms);
	char buf[8];

	Position test; test.width = NO; test.height = NO; test.type = NO;

	for(int i=0; i<9999; i++)
	{
		if(test.type==NO)
		{
			random=(rand() % safe_rooms); 
			yx=RandomCoords(yx, safe_rooms, room.Area[random]);
			test.yPos = yx[0]; test.xPos = yx[1];
			test = CheckArea(test,'.');		 	
		} else
		{
			break;
		}
	}
	if(yx[0] < 1 || yx[0] >= MAX_Y - 1) yx[0] = 2;
	if(yx[1] < 1 || yx[1] >= MAX_X - 1) yx[1] = 2;
	monster->Position.yPos = yx[0]; 
	monster->Position.xPos = yx[1]; 
	// Save the initial character value!
	monster->Position.ch = mvinch(monster->Position.yPos,monster->Position.xPos);
	monster->Position.c = mvinch(monster->Position.yPos,monster->Position.xPos);
	
	sprintf(buf,"%c",monster->symbol);
	mvprintw(monster->Position.yPos, monster->Position.xPos, "%s", buf);
	refresh();


	return yx;
}

Entity *MonsterMove(Level level, Entity *monster, char c1, char c2, int nexty, int nextx, int mode)
{
	int y,x = 0;
	y = monster->Position.yPos; x = monster->Position.xPos;
	if(y < 0 || y >= MAX_Y || x < 0 || x >= MAX_X || nexty < 0 || nexty >= MAX_Y || nextx < 0 || nextx >= MAX_X)
	{
		return monster;
	}

if(monster->alive==YES)
{
	if(mode==YES)
	{
		PrintChar(monster->Position.yPos,monster->Position.xPos, c1);
		monster->Position.yPos = nexty; monster->Position.xPos = nextx; y = nexty; x = nextx;
		PrintChar(y,x, c2);
	} else if(mode==DOOR)
	{
		PrintChar(y,x,level.map_backup[y][x]);
		monster->Position.yPos = nexty; monster->Position.xPos = nextx; y = nexty; x = nextx;
		PrintChar(y,x,monster->symbol);	
	} else if(mode==FOLLOW)
	{
		if(level.map_backup[y][x]=='+')
		{
			PrintChar(y,x,':');
			level.map_backup[y][x]=':';
		} else
		{
			PrintChar(y,x,level.map_backup[y][x]);
		} // AS THERE WILL BE SPACE LEFT BETWEEN MONSTER AND PLAYER:	
		if((nexty-y)==2)
		{
			y++;
		}	
		if((nexty-y)==-2)
		{
			y--;
		}	
		if((nextx-x)==2)
		{
			x++;
		}	
		if((nextx-x)==-2)
		{
			x--;
		}	
		monster->Position.yPos = y; monster->Position.xPos = x;
		PrintChar(y,x,monster->symbol);
	}
}
	return monster;
}

