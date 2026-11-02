#include "global.h"
#include "intro.h"

int IntroScreen()
{
#ifdef __EMSCRIPTEN__
	clear();
	mvprintw(2, 2, "========================================");
	mvprintw(3, 2, "   YOU ARE WHAT YOU EAT - ROGUE");
	mvprintw(4, 2, "========================================");
	mvprintw(6, 2, "A tactical roguelike where you polymorph");
	mvprintw(7, 2, "directly into whatever beast you devour!");
	mvprintw(9, 2, "* Devour corpses to inherit stats & traits.");
	mvprintw(10, 2, "* Beware: older corpses rot and cause sickness!");
	mvprintw(11, 2, "* Starvation will kill you - hunt wisely.");
	mvprintw(13, 2, "Controls:");
	mvprintw(14, 4, "Move:    D-Pad / WASD / Numpad");
	mvprintw(15, 4, "Ability: 'f' / [Abil] (Form active)");
	mvprintw(16, 4, "Eat:     'e' or 'E' / [Eat]");
	mvprintw(17, 4, "Bestiary:'B' / [Bestiary] (Form Compendium)");
	mvprintw(18, 4, "Wait:    '.' or Space / [Next] | Close: 'c'");
	mvprintw(19, 2, "Active Seed: %s (%s)",
		GAME_SEED_LABEL,
		GAME_SEED_MODE == 1 ? "DAILY CHALLENGE" : (GAME_SEED_MODE == 2 ? "CUSTOM SEED" : "RANDOM RUN"));
	mvprintw(21, 2, ">> Tap [ Next ] or press any key to enter <<");
	refresh();
	getch();
	return 1;
#else
	clear();
	mvprintw(1,0,"%s",intro0);
	printw("%s",intro1);
	printw("%s",intro2);
	
	printw("\n\n\n\n\n                                                                       PRESS A KEY TO CONTINUE\n");

	getch();

	return 1;
#endif
}

void ClearMsg(void)
{
	move(0, 0);
	clrtoeol();
}

void PlaySound(const char *sfx)
{
#ifdef __EMSCRIPTEN__
	if(sfx != NULL)
	{
		web_play_sfx(sfx);
	}
#else
	(void)sfx;
#endif
}

int WelcomeTxt()
{
	// WELCOME TEXT + INSTRUCTIONS:
	ClearMsg();
	mvprintw(0,0, "Move: wasd/qezx | Look: 'v' | Pack: 'i' | Bestiary: 'B' | Pick: 'g' | Abil: 'f' | Eat: 'E'");
	refresh(); getch();
	ClearMsg();

	return 1;
}

int PlayerInfoBar(Level *level)
{
	char *hunger_status = "Normal";
	int h = level[level->lvl].player->hunger;
	if(h >= 750)
	{
		hunger_status = "Satiated";
	} else if(h >= 350)
	{
		hunger_status = "Normal";
	} else if(h >= 150)
	{
		hunger_status = "Hungry";
	} else if(h > 0)
	{
		hunger_status = "Weak";
	} else
	{
		hunger_status = "Starving";
	}

	int total_att = GetPlayerAttack(level[level->lvl].player);
	int total_def = GetPlayerDefense(level[level->lvl].player);

	move(MAX_Y, 0);
	clrtoeol();
	mvprintw(MAX_Y, 0,"    Player: %s (%c)", level[level->lvl].player->name, level->player_symbol);
	printw("    Hp: %d/%d", level[level->lvl].player->health, level[level->lvl].player->maxhp);
	
	int ew = level[level->lvl].player->equipped_weapon;
	if(level[level->lvl].player->can_open_doors == YES && ew >= 0 && ew < level[level->lvl].player->num_items)
	{
		printw("    Att: %d [%s]", total_att, level[level->lvl].player->inventory[ew].name);
	}
	else
	{
		printw("    Att: %d", total_att);
	}

	int ea = level[level->lvl].player->equipped_armor;
	if(level[level->lvl].player->can_open_doors == YES && ea >= 0 && ea < level[level->lvl].player->num_items)
	{
		printw("    Def: %d [%s]", total_def, level[level->lvl].player->inventory[ea].name);
	}
	else if(total_def > 0)
	{
		printw("    Def: %d", total_def);
	}

	printw("    Lvl: %d", level->lvl);
	printw("    Hunger: %s (%d)", hunger_status, level[level->lvl].player->hunger);
	if(level[level->lvl].player->poison_turns > 0)
	{
		printw(" [POISON:%d]", level[level->lvl].player->poison_turns);
	}
	if(level[level->lvl].player->regen_rate > 0)
	{
		printw(" [REGEN]");
	}
	if(level[level->lvl].player->can_phase_walls > 0)
	{
		printw(" [PHASE]");
	}
	if(level[level->lvl].player->acid_corrosion > 0)
	{
		printw(" [CAUSTIC]");
	}
	if(level[level->lvl].player->evasion_chance > 0)
	{
		printw(" [EVA:%d%%]", level[level->lvl].player->evasion_chance);
	}
	if(level[level->lvl].player->poison_attack > 0)
	{
		printw(" [VENOM]");
	}
	if(level[level->lvl].player->scavenger_bonus > 0)
	{
		printw(" [SCAV:+%d%%]", level[level->lvl].player->scavenger_bonus);
	}
	if(level[level->lvl].player->life_drain > 0)
	{
		printw(" [DRAIN:+%d]", level[level->lvl].player->life_drain);
	}
	if(level[level->lvl].player->ability_name != NULL)
	{
		if(level[level->lvl].player->ability_cd > 0)
		{
			printw(" [%s:%dt]", level[level->lvl].player->ability_name, level[level->lvl].player->ability_cd);
		}
		else
		{
			printw(" [%s:READY]", level[level->lvl].player->ability_name);
		}
	}
	printw("    Inv: %d/10", level[level->lvl].player->num_items);
	printw("    XP: %d (Rk:%d)", level[level->lvl].player->xp, (level[level->lvl].player->xp / 20) + 1);
	if(GAME_SEED_MODE == 1)
	{
		printw(" [Seed:DAILY-%s]", GAME_SEED_LABEL);
	}
	else if(GAME_SEED_MODE == 2)
	{
		printw(" [Seed:%s]", GAME_SEED_LABEL);
	}
	else
	{
		printw(" [Seed:#%u]", GAME_SEED % 1000000u);
	}

	int py = level[level->lvl].player->Position.yPos;
	int px = level[level->lvl].player->Position.xPos;
	Item *floor_it = GetItemAt(level[level->lvl], py, px);
	Corpse *floor_corp = GetCorpseAt(level[level->lvl], py, px);
	char under_tile = (level[level->lvl].map_backup != NULL) ? level[level->lvl].map_backup[py][px] : '.';
	if (under_tile == '*')
	{
		printw("  [Floor: Golden Portal (*)]");
	}
	else if (floor_it != NULL && floor_it->active == YES)
	{
		printw("  [Floor: %s]", floor_it->name);
	}
	else if (floor_corp != NULL && floor_corp->active == YES)
	{
		printw("  [Floor: %s Corpse]", floor_corp->name);
	}
	else if (under_tile == '<')
	{
		printw("  [Floor: Downstairs (<)]");
	}
	else if (under_tile == '>')
	{
		printw("  [Floor: Upstairs (>)]");
	}
	else if (under_tile == '_')
	{
		printw("  [Floor: Metabolic Altar (_)]");
	}
	else if (under_tile == '~')
	{
		printw("  [Floor: Subterranean Spring (~)]");
	}
	else if (under_tile == '^')
	{
		printw("  [Floor: Sticky Web (^)]");
	}
	else if (under_tile == '=')
	{
		printw("  [Floor: Acid Puddle (=)]");
	}
	printw("      ");

	move(level[level->lvl].player->Position.yPos, level[level->lvl].player->Position.xPos);

	return 1;
}

void SpawnFloorItems(Level *level, Entity rooms, int lvl)
{
	if(rooms.mode <= 0) return;
	int spawn_count = (rand() % 3) + 2; // 2 to 4 items per level
	int *yx = malloc(sizeof(int) * 3);

	for(int i = 0; i < spawn_count && level->num_items < level->max_items; i++)
	{
		int random_room = rand() % rooms.mode;
		yx = RandomCoords(yx, rooms.mode, rooms.Area[random_room]);
		int y = yx[0];
		int x = yx[1];

		// Only place on free floor (and never overwrite player/stairs/monsters/other items):
		if(y < 1 || y >= MAX_Y - 1 || x < 1 || x >= MAX_X - 1 ||
		   level->map_backup[y][x] != '.' ||
		   (level->player != NULL && level->player->Position.yPos == y && level->player->Position.xPos == x) ||
		   GetMonsterAt(y, x, *level) != NULL ||
		   GetItemAt(*level, y, x) != NULL)
		{
			continue;
		}

		int idx = level->num_items;
		level->items[idx].Position.yPos = y;
		level->items[idx].Position.xPos = x;
		level->items[idx].active = YES;
		level->items[idx].equipped = NO;

		int roll;
		if(lvl <= 2)
		{
			int pool[4] = {0, 1, 4, 5};
			roll = pool[rand() % 4];
		}
		else if(lvl <= 5)
		{
			int pool[6] = {1, 2, 3, 4, 5, 6};
			roll = pool[rand() % 6];
		}
		else
		{
			int pool[6] = {2, 3, 5, 6, 7, 8};
			roll = pool[rand() % 6];
		}

		if(roll == 0)
		{
			level->items[idx].symbol = ')';
			level->items[idx].name = "Iron Dagger";
			level->items[idx].bonus_att = 2;
			level->items[idx].bonus_def = 0;
			level->items[idx].item_type = ITEM_WEAPON;
		}
		else if(roll == 1)
		{
			level->items[idx].symbol = ')';
			level->items[idx].name = "Steel Shortsword";
			level->items[idx].bonus_att = 4;
			level->items[idx].bonus_def = 0;
			level->items[idx].item_type = ITEM_WEAPON;
		}
		else if(roll == 2)
		{
			level->items[idx].symbol = ')';
			level->items[idx].name = "Heavy Broadsword";
			level->items[idx].bonus_att = 6;
			level->items[idx].bonus_def = 0;
			level->items[idx].item_type = ITEM_WEAPON;
		}
		else if(roll == 3)
		{
			level->items[idx].symbol = ')';
			level->items[idx].name = "Trollish Warhammer";
			level->items[idx].bonus_att = 8;
			level->items[idx].bonus_def = 0;
			level->items[idx].item_type = ITEM_WEAPON;
		}
		else if(roll == 4)
		{
			level->items[idx].symbol = '[';
			level->items[idx].name = "Leather Tunic";
			level->items[idx].bonus_att = 0;
			level->items[idx].bonus_def = 2;
			level->items[idx].item_type = ITEM_ARMOR;
		}
		else if(roll == 5)
		{
			level->items[idx].symbol = '[';
			level->items[idx].name = "Iron Chainmail";
			level->items[idx].bonus_att = 0;
			level->items[idx].bonus_def = 4;
			level->items[idx].item_type = ITEM_ARMOR;
		}
		else if(roll == 6)
		{
			level->items[idx].symbol = '[';
			level->items[idx].name = "Steel Plate Armor";
			level->items[idx].bonus_att = 0;
			level->items[idx].bonus_def = 6;
			level->items[idx].item_type = ITEM_ARMOR;
		}
		else if(roll == 7)
		{
			level->items[idx].symbol = ')';
			level->items[idx].name = "Runed Greatsword";
			level->items[idx].bonus_att = 10;
			level->items[idx].bonus_def = 0;
			level->items[idx].item_type = ITEM_WEAPON;
		}
		else
		{
			level->items[idx].symbol = '[';
			level->items[idx].name = "Dragonscale Mail";
			level->items[idx].bonus_att = 0;
			level->items[idx].bonus_def = 8;
			level->items[idx].item_type = ITEM_ARMOR;
		}

		level->map_backup[y][x] = level->items[idx].symbol;
		PrintChar(y, x, level->items[idx].symbol);
		level->num_items++;
	}

	free(yx);
}

void SpawnFloorShrines(Level *level, Entity rooms, int lvl)
{
	// Spawn rare Metabolic Altars (_) and Subterranean Springs (~) on mid-floors (lvl 2-9, guaranteed 2 on lvl 4-8):
	if(lvl < 2 || lvl > 9 || rooms.mode <= 0)
	{
		return;
	}

	int count = (lvl >= 4 && lvl <= 8) ? 2 : 1;
	int *yx = malloc(sizeof(int) * 3);

	for(int i = 0; i < count; i++)
	{
		char shrine_sym = ((lvl + i) % 2 == 0) ? '_' : '~';
		for(int attempt = 0; attempt < 25; attempt++)
		{
			int random_room = rand() % rooms.mode;
			yx = RandomCoords(yx, rooms.mode, rooms.Area[random_room]);
			int y = yx[0];
			int x = yx[1];

			if(y > 1 && y < MAX_Y - 1 && x > 1 && x < MAX_X - 1 &&
			   level->map_backup[y][x] == '.' &&
			   (level->player == NULL || level->player->Position.yPos != y || level->player->Position.xPos != x) &&
			   GetMonsterAt(y, x, *level) == NULL &&
			   GetItemAt(*level, y, x) == NULL)
			{
				level->map_backup[y][x] = shrine_sym;
				PrintChar(y, x, shrine_sym);
				break;
			}
		}
	}

	free(yx);
}

void SpawnFloorHazards(Level *level, Entity rooms, int lvl)
{
	// Spawn environmental hazards: Sticky Spider Webs (^) and Caustic Acid Puddles (=)
	if(lvl < 1 || rooms.mode <= 0)
	{
		return;
	}

	int count = 2 + (lvl / 2);
	if(count > 8) count = 8;
	int *yx = malloc(sizeof(int) * 3);

	for(int i = 0; i < count; i++)
	{
		char haz_sym = '^';
		if(lvl <= 2)
		{
			haz_sym = ((rand() % 4) == 0) ? '=' : '^';
		}
		else
		{
			haz_sym = ((rand() % 2) == 0) ? '=' : '^';
		}

		for(int attempt = 0; attempt < 20; attempt++)
		{
			int random_room = rand() % rooms.mode;
			yx = RandomCoords(yx, rooms.mode, rooms.Area[random_room]);
			int y = yx[0];
			int x = yx[1];

			if(y > 1 && y < MAX_Y - 1 && x > 1 && x < MAX_X - 1 &&
			   level->map_backup[y][x] == '.' &&
			   (level->player == NULL || level->player->Position.yPos != y || level->player->Position.xPos != x) &&
			   GetMonsterAt(y, x, *level) == NULL &&
			   GetItemAt(*level, y, x) == NULL)
			{
				level->map_backup[y][x] = haz_sym;
				PrintChar(y, x, haz_sym);
				break;
			}
		}
	}

	free(yx);
}

void TriggerFloorHazard(Level *leveli, Level level, Entity *entity, int is_player, int y, int x)
{
	if(leveli == NULL || entity == NULL) return;
	Level *cur = &leveli[leveli->lvl];
	if(cur->map_backup == NULL || y < 0 || y >= MAX_Y || x < 0 || x >= MAX_X) return;

	char tile = cur->map_backup[y][x];
	if(is_player)
	{
		if(tile == '^')
		{
			if(entity->symbol == 'x' || entity->can_phase_walls > 0)
			{
				ClearMsg();
				mvprintw(0, 0, "You skitter effortlessly across the Sticky Spider Web (^).");
				refresh();
			}
			else
			{
				PlaySound("miss");
				entity->hunger -= 20;
				if(entity->hunger < 0) entity->hunger = 0;
				cur->map_backup[y][x] = '.';
				entity->Position.ch = '.';
				ClearMsg();
				mvprintw(0, 0, "You tear through a Sticky Spider Web (^), burning 20 hunger to break free!");
				PlayerInfoBar(leveli);
				refresh();
			}
		}
		else if(tile == '=')
		{
			if(entity->symbol == 'B')
			{
				PlaySound("shrine");
				entity->health += 2;
				if(entity->health > entity->maxhp) entity->health = entity->maxhp;
				entity->hunger += 30;
				if(entity->hunger > entity->max_hunger) entity->hunger = entity->max_hunger;
				cur->map_backup[y][x] = '.';
				entity->Position.ch = '.';
				ClearMsg();
				mvprintw(0, 0, "Your gelatinous body absorbs the Caustic Acid Puddle (=)! (+2 HP, +30 hunger)");
				PlayerInfoBar(leveli);
				refresh();
			}
			else if(entity->symbol == 'D' || entity->can_phase_walls > 0)
			{
				ClearMsg();
				mvprintw(0, 0, "You pass unscathed over the Caustic Acid Puddle (=).");
				refresh();
			}
			else
			{
				PlaySound("hurt");
				entity->health -= 2;
				int psn = 0;
				if(entity->symbol != 's' && entity->symbol != 'L')
				{
					entity->poison_turns += 2;
					psn = 1;
				}
				cur->map_backup[y][x] = '.';
				entity->Position.ch = '.';
				ClearMsg();
				mvprintw(0, 0, "SPLASH! Caustic Acid Puddle (=) burns you for 2 dmg!%s", psn ? " (Poisoned 2t!)" : "");
				if(entity->health <= 0)
				{
					entity->alive = DEAD;
					leveli->killer = "Caustic Acid Puddle";
				}
				PlayerInfoBar(leveli);
				refresh();
			}
		}
	}
	else
	{
		int is_vis = (cur->visible != NULL && y >= 0 && y < MAX_Y && x >= 0 && x < MAX_X) ? cur->visible[y][x] : 0;
		if(tile == '^')
		{
			if(entity->symbol != 'x' && entity->can_phase_walls == 0)
			{
				entity->snare_turns += 3;
				cur->map_backup[y][x] = '.';
				if(is_vis)
				{
					ClearMsg();
					mvprintw(0, 0, "The %s gets entangled in a Sticky Web (^) for 3 turns!", entity->name);
					refresh();
				}
			}
		}
		else if(tile == '=')
		{
			if(entity->symbol != 'B' && entity->symbol != 'D' && entity->can_phase_walls == 0)
			{
				entity->health -= 3;
				if(entity->symbol != 's' && entity->symbol != 'L')
				{
					entity->poison_turns += 2;
				}
				cur->map_backup[y][x] = '.';
				if(entity->health <= 0)
				{
					int old_xp = cur->player->xp;
					entity->alive = DEAD;
					cur->player->xp += entity->attack;
					CheckXPUpgrade(cur->player, old_xp);
					DropCorpse(leveli, entity, y, x);
					entity->Position.yPos = -1;
					entity->Position.xPos = -1;
					if(is_vis)
					{
						ClearMsg();
						mvprintw(0, 0, "The %s dissolves in a Caustic Acid Puddle (=)! (+%d XP)", entity->name, entity->attack);
						refresh();
					}
				}
				else if(is_vis)
				{
					ClearMsg();
					mvprintw(0, 0, "The %s splashes into an Acid Puddle (=) for 3 acid dmg! [%d/%d HP]", entity->name, entity->health, entity->maxhp);
					refresh();
				}
			}
		}
	}
}

int UseShrine(Level *leveli, Level level, Entity *player, int y, int x)
{
	char tile = (leveli[leveli->lvl].map_backup != NULL) ? leveli[leveli->lvl].map_backup[y][x] : '.';
	if(tile == '_')
	{
		ClearMsg();
		mvprintw(0, 0, "Metabolic Altar (_): Purify all corpses, cure poison, and empower (+10 XP, +5 HP)? (y/n)");
		refresh();
		int ans = getch();
		ClearMsg();
		if(ans == 'y' || ans == 'Y')
		{
			int purified = 0;
			for(int i = 0; i < player->num_items; i++)
			{
				if(player->inventory[i].item_type == ITEM_CORPSE)
				{
					player->inventory[i].corpse_data.age = 0;
					purified++;
				}
			}
			for(int i = 0; i < leveli[leveli->lvl].num_corpses; i++)
			{
				if(leveli[leveli->lvl].corpses[i].active == YES)
				{
					int cy = leveli[leveli->lvl].corpses[i].Position.yPos;
					int cx = leveli[leveli->lvl].corpses[i].Position.xPos;
					if(abs(cy - y) <= 2 && abs(cx - x) <= 2)
					{
						leveli[leveli->lvl].corpses[i].age = 0;
						purified++;
					}
				}
			}
			player->poison_turns = 0;
			player->health += 5;
			if(player->health > player->maxhp) player->health = player->maxhp;
			player->hunger += 150;
			if(player->hunger > player->max_hunger) player->hunger = player->max_hunger;

			int old_xp = player->xp;
			player->xp += 10;
			int ranked_up = CheckXPUpgrade(player, old_xp);

			leveli[leveli->lvl].map_backup[y][x] = '.';
			player->Position.ch = '.';

			if(ranked_up)
			{
				mvprintw(0, 0, "Altar invoked! Purified %d corpses, +5 HP, +150 hunger, +10 XP! [RANK UP! Rk:%d (+3 MaxHP, +1 ATK)]",
					purified, (player->xp / 20) + 1);
			}
			else
			{
				PlaySound("shrine");
				mvprintw(0, 0, "Altar invoked! Purified %d corpses to Fresh, cured poison, +5 HP, +150 hunger, +10 XP!", purified);
			}
			PlayerInfoBar(leveli);
			refresh();
			return 1;
		}
		else
		{
			player->Position.ch = '_';
			mvprintw(0, 0, "You leave the Metabolic Altar (_) untouched for now. (Press 'r' to invoke)");
			refresh();
			return 0;
		}
	}
	else if(tile == '~')
	{
		ClearMsg();
		mvprintw(0, 0, "Subterranean Spring (~): Drink to restore +450 hunger, +10 HP, and reset cooldown? (y/n)");
		refresh();
		int ans = getch();
		ClearMsg();
		if(ans == 'y' || ans == 'Y')
		{
			player->hunger += 450;
			if(player->hunger > player->max_hunger) player->hunger = player->max_hunger;
			player->health += 10;
			if(player->health > player->maxhp) player->health = player->maxhp;
			player->poison_turns = 0;
			player->ability_cd = 0;

			for(int i = 0; i < player->num_items; i++)
			{
				if(player->inventory[i].item_type == ITEM_CORPSE)
				{
					player->inventory[i].corpse_data.age -= 60;
					if(player->inventory[i].corpse_data.age < 0) player->inventory[i].corpse_data.age = 0;
				}
			}

			leveli[leveli->lvl].map_backup[y][x] = '.';
			player->Position.ch = '.';

			PlaySound("shrine");
			mvprintw(0, 0, "You drink from the Subterranean Spring! (+450 hunger, +10 HP, ability ready, pack cooled!)");
			PlayerInfoBar(leveli);
			refresh();
			return 1;
		}
		else
		{
			player->Position.ch = '~';
			mvprintw(0, 0, "You step past the Subterranean Spring (~). (Press 'r' to drink)");
			refresh();
			return 0;
		}
	}
	return 0;
}

Item *GetItemAt(Level level, int y, int x)
{
	for(int i = 0; i < level.num_items; i++)
	{
		if(level.items[i].active == YES &&
		   level.items[i].Position.yPos == y &&
		   level.items[i].Position.xPos == x)
		{
			return &level.items[i];
		}
	}
	return NULL;
}

int PickUpFloorItem(Level *leveli, Level level, Entity *player)
{
	int py = player->Position.yPos;
	int px = player->Position.xPos;

	if(player->num_items >= 10)
	{
		ClearMsg();
		mvprintw(0, 0, "Your backpack is full! (10/10 items)");
		refresh();
		return 0;
	}

	// 1. Check for floor item (')' or '[')
	Item *it = GetItemAt(level, py, px);
	if(it != NULL && it->active == YES)
	{
		int slot = player->num_items;
		player->inventory[slot] = *it;
		player->inventory[slot].active = YES;
		player->inventory[slot].equipped = NO;
		player->num_items++;

		it->active = NO;
		if(leveli[leveli->lvl].map_backup[py][px] == ')' || leveli[leveli->lvl].map_backup[py][px] == '[')
		{
			leveli[leveli->lvl].map_backup[py][px] = '.';
		}
		player->Position.ch = leveli[leveli->lvl].map_backup[py][px];

		ClearMsg();
		PlaySound("pickup");
		if(it->item_type == ITEM_WEAPON)
		{
			mvprintw(0, 0, "Picked up %s (+%d ATK)! Press 'i' for pack.", it->name, it->bonus_att);
		}
		else
		{
			mvprintw(0, 0, "Picked up %s (+%d DEF)! Press 'i' for pack.", it->name, it->bonus_def);
		}
		PlayerInfoBar(leveli);
		refresh();
		return 1;
	}

	// 2. Check for corpse ('%')
	Corpse *c = GetCorpseAt(level, py, px);
	if(c != NULL && c->active == YES)
	{
		int slot = player->num_items;
		player->inventory[slot].symbol = '%';
		player->inventory[slot].name = c->name;
		player->inventory[slot].bonus_att = c->attack;
		player->inventory[slot].bonus_def = 0;
		player->inventory[slot].item_type = ITEM_CORPSE;
		player->inventory[slot].equipped = NO;
		player->inventory[slot].active = YES;
		player->inventory[slot].corpse_data = *c;
		player->num_items++;

		c->active = NO;
		if(leveli[leveli->lvl].map_backup[py][px] == '%')
		{
			leveli[leveli->lvl].map_backup[py][px] = '.';
		}
		player->Position.ch = leveli[leveli->lvl].map_backup[py][px];

		ClearMsg();
		PlaySound("pickup");
		mvprintw(0, 0, "Stowed %s corpse into backpack! (Slot %d) Press 'E' or 'i' to eat anytime.", c->name, slot + 1);
		PlayerInfoBar(leveli);
		refresh();
		return 1;
	}

	ClearMsg();
	mvprintw(0, 0, "There is nothing here on the floor to pick up.");
	refresh();
	return 0;
}

int ShowInventory(Level *leveli, Level level, Entity *player)
{
	int selected = 0;
	char inv_msg[128] = "";

	while (1)
	{
		clear();
		mvprintw(1, 2, "==========================================================");
		mvprintw(2, 2, "   BACKPACK & INVENTORY (10 Slots Total)");
		mvprintw(3, 2, "==========================================================");

		if (inv_msg[0] != '\0')
		{
			mvprintw(4, 2, ">> %s", inv_msg);
		}
		else
		{
			mvprintw(4, 2, " ");
		}

		for (int i = 0; i < 10; i++)
		{
			char p1 = (i == selected) ? '-' : ' ';
			char p2 = (i == selected) ? '>' : ' ';

			if (i < player->num_items)
			{
				Item *it = &player->inventory[i];
				char tag[32] = "";
				if (it->equipped == YES)
				{
					if (it->item_type == ITEM_WEAPON) sprintf(tag, " [WIELDED]");
					else if (it->item_type == ITEM_ARMOR) sprintf(tag, " [EQUIPPED]");
				}

				if (it->item_type == ITEM_WEAPON)
				{
					mvprintw(5 + i, 2, "%c%c [Slot %2d] %s (+%d ATK)%s", p1, p2, i + 1, it->name, it->bonus_att, tag);
				}
				else if (it->item_type == ITEM_ARMOR)
				{
					mvprintw(5 + i, 2, "%c%c [Slot %2d] %s (+%d DEF)%s", p1, p2, i + 1, it->name, it->bonus_def, tag);
				}
				else if (it->item_type == ITEM_CORPSE)
				{
					const char *st = GetCorpseStage(it->corpse_data.age);
					mvprintw(5 + i, 2, "%c%c [Slot %2d] %s %s Corpse (%d nut) [EATABLE]", p1, p2, i + 1, st, it->name, it->corpse_data.nutrition);
				}
			}
			else
			{
				mvprintw(5 + i, 2, "%c%c [Slot %2d] ", p1, p2, i + 1);
			}
		}

		mvprintw(16, 2, "----------------------------------------------------------");
		mvprintw(17, 2, "Current Form: %s (%c) | Hands: %s", player->name, player->symbol, (player->can_open_doors == YES ? "YES (Can wield/wear)" : "NO (Beast - cannot wield)"));
		mvprintw(18, 2, "Controls:");
		mvprintw(19, 4, "[Up/Down / w/s / 8/2]: Move selection (Current: Slot %d)", selected + 1);
		mvprintw(20, 4, "[Enter / Space / 'e' / 'f']: Use/Equip/Eat selected item");
		mvprintw(21, 4, "['g' / 'd' / Pick]: Drop selected item to floor");
		mvprintw(22, 4, "['i' / 'q' / 'c' / Esc]: Exit backpack");
		refresh();

		int ch = getch();

		// EXIT INVENTORY: 'i', 'I', 'q', 'Q', 'c', 'C', 'x', 'X', 27 (Esc)
		if (ch == 'i' || ch == 'I' || ch == 'q' || ch == 'Q' || ch == 'c' || ch == 'C' || ch == 'x' || ch == 'X' || ch == 27)
		{
			break;
		}
		// Navigate Up:
		else if (ch == 'w' || ch == 'W' || ch == 'k' || ch == 'K' || ch == '8')
		{
			selected--;
			if (selected < 0) selected = 9;
		}
		// Navigate Down:
		else if (ch == 's' || ch == 'S' || ch == 'j' || ch == 'J' || ch == '2')
		{
			selected++;
			if (selected > 9) selected = 0;
		}
		// Number keys 1-9, 0 to select slot directly:
		else if (ch >= '1' && ch <= '9')
		{
			selected = ch - '1';
		}
		else if (ch == '0')
		{
			selected = 9;
		}
		// DROP SELECTED ITEM: 'g', 'G', 'd', 'D', ','
		else if (ch == 'g' || ch == 'G' || ch == 'd' || ch == 'D' || ch == ',')
		{
			if (selected < player->num_items)
			{
				int py = player->Position.yPos;
				int px = player->Position.xPos;
				Item it = player->inventory[selected];

				if (selected == player->equipped_weapon) player->equipped_weapon = -1;
				else if (player->equipped_weapon > selected) player->equipped_weapon--;

				if (selected == player->equipped_armor) player->equipped_armor = -1;
				else if (player->equipped_armor > selected) player->equipped_armor--;

				for (int k = selected; k < player->num_items - 1; k++)
				{
					player->inventory[k] = player->inventory[k + 1];
				}
				player->num_items--;

				if (it.item_type == ITEM_CORPSE)
				{
					Entity dummy;
					dummy.mode = 0;
					dummy.symbol = it.corpse_data.symbol;
					dummy.name = it.corpse_data.name;
					dummy.maxhp = it.corpse_data.maxhp;
					dummy.attack = it.corpse_data.attack;
					DropCorpse(leveli, &dummy, py, px);
					int c_idx = leveli[leveli->lvl].num_corpses - 1;
					if (c_idx >= 0)
					{
						leveli[leveli->lvl].corpses[c_idx].age = it.corpse_data.age;
						leveli[leveli->lvl].corpses[c_idx].nutrition = it.corpse_data.nutrition;
					}
				}
				else
				{
					int lidx = leveli[leveli->lvl].num_items;
					if (lidx < leveli[leveli->lvl].max_items)
					{
						leveli[leveli->lvl].items[lidx] = it;
						leveli[leveli->lvl].items[lidx].Position.yPos = py;
						leveli[leveli->lvl].items[lidx].Position.xPos = px;
						leveli[leveli->lvl].items[lidx].active = YES;
						leveli[leveli->lvl].items[lidx].equipped = NO;
						leveli[leveli->lvl].num_items++;
					}
					char cur_tile = leveli[leveli->lvl].map_backup[py][px];
					if (cur_tile != '<' && cur_tile != '>' && cur_tile != ':' && cur_tile != '+' && cur_tile != '*' && cur_tile != '_' && cur_tile != '~')
					{
						leveli[leveli->lvl].map_backup[py][px] = it.symbol;
					}
				}
				player->Position.ch = leveli[leveli->lvl].map_backup[py][px];
				snprintf(inv_msg, sizeof(inv_msg), "Dropped %s on the floor.", it.name);
			}
			else
			{
				snprintf(inv_msg, sizeof(inv_msg), "Slot %d is empty, nothing to drop.", selected + 1);
			}
		}
		// USE / EQUIP / EAT: Enter (10/13), Space (' '), 'e', 'E', 'f', 'F'
		else if (ch == 10 || ch == 13 || ch == ' ' || ch == 'e' || ch == 'E' || ch == 'f' || ch == 'F')
		{
			if (selected < player->num_items)
			{
				Item *it = &player->inventory[selected];
				if (it->item_type == ITEM_WEAPON)
				{
					if (player->can_open_doors == NO)
					{
						snprintf(inv_msg, sizeof(inv_msg), "Your %s body lacks hands to wield weapons!", player->name);
					}
					else
					{
						if (it->equipped == YES)
						{
							it->equipped = NO;
							player->equipped_weapon = -1;
							snprintf(inv_msg, sizeof(inv_msg), "Sheathed %s.", it->name);
						}
						else
						{
							if (player->equipped_weapon >= 0)
							{
								player->inventory[player->equipped_weapon].equipped = NO;
							}
							it->equipped = YES;
							player->equipped_weapon = selected;
							snprintf(inv_msg, sizeof(inv_msg), "Wielded %s (+%d ATK)!", it->name, it->bonus_att);
						}
					}
				}
				else if (it->item_type == ITEM_ARMOR)
				{
					if (player->can_open_doors == NO)
					{
						snprintf(inv_msg, sizeof(inv_msg), "Your %s body lacks hands to wear armor!", player->name);
					}
					else
					{
						if (it->equipped == YES)
						{
							it->equipped = NO;
							player->equipped_armor = -1;
							snprintf(inv_msg, sizeof(inv_msg), "Took off %s.", it->name);
						}
						else
						{
							if (player->equipped_armor >= 0)
							{
								player->inventory[player->equipped_armor].equipped = NO;
							}
							it->equipped = YES;
							player->equipped_armor = selected;
							snprintf(inv_msg, sizeof(inv_msg), "Equipped %s (+%d DEF)!", it->name, it->bonus_def);
						}
					}
				}
				else if (it->item_type == ITEM_CORPSE)
				{
					Corpse saved_corpse = it->corpse_data;
					if (player->equipped_weapon > selected) player->equipped_weapon--;
					if (player->equipped_armor > selected) player->equipped_armor--;
					for (int k = selected; k < player->num_items - 1; k++)
					{
						player->inventory[k] = player->inventory[k + 1];
					}
					player->num_items--;
					EatCorpse(leveli, level, player, &saved_corpse);
					break; // Exit inventory after eating / polymorphing
				}
			}
			else
			{
				snprintf(inv_msg, sizeof(inv_msg), "Slot %d is empty.", selected + 1);
			}
		}
	}

	ClearMsg();
	if (inv_msg[0] != '\0')
	{
		mvprintw(0, 0, "%s", inv_msg);
	}
	UpdateFOV(&leveli[leveli->lvl]);
	RenderFOV(&leveli[leveli->lvl]);
	PlayerInfoBar(leveli);
	refresh();
	return 1;
}

typedef struct {
	char symbol;
	const char *name;
	const char *floors;
	int base_hp;
	int base_att;
	int poly_hp;
	int nutrition;
	int hunger_rate;
	int can_gear;
	const char *ability_name;
	int ability_cd;
	int ability_cost;
	const char *ability_desc;
	const char *trait_desc;
	int seen;
	int kills;
	int devoured;
	int js_seen_synced;
} BestiaryEntry;

#define BESTIARY_COUNT 10

static BestiaryEntry g_bestiary[BESTIARY_COUNT] = {
	{ '@', "Human",            "Floor 0 (Start)", 20, 1,  20,    0, 1, 1, "Bandage",       10, 35, "Bind wounds to restore +6 HP.", "Balanced Human Form (Starts with Iron Dagger +2 ATK)", 1, 0, 1, 0 },
	{ 'x', "Spider",           "Floors 0-2",       2, 1,  14,  150, 2, 0, "Spin Web",       6, 40, "Ensnares foes in 2 tiles (3t, 1 dmg) & places Sticky Webs (^).", "Skitter (40% 2nd hit), Fast Metabolism (2/t), Web Immune, No Gear", 0, 0, 0, 0 },
	{ 's', "Snake",            "Floors 0-6",       3, 2,  18,  250, 1, 0, "Venom Spit",     5, 30, "Spits venom up to 4 tiles for ATK+2 dmg & 4t poison.", "Venomous Strike (3t poison), Poison & Tainted Meat Immune, No Gear", 0, 0, 0, 0 },
	{ 'V', "Vampire Bat",      "Floors 0-6, 13+",  2, 2,  14,  100, 2, 0, "Echo Screech",   6, 30, "Ultrasonic pulse reveals all monsters & tiles across the floor.", "Flight (50% Evasion, FOV 12), Life Drain (+1 HP), Fast Burn (2/t), No Gear", 0, 0, 0, 0 },
	{ 'g', "Goblin",           "Floors 3-13+",     5, 3,  26,  400, 1, 1, "Flashbang",      8, 35, "Blinds & ensnares foes within 2 tiles for 2t + 2 dmg.", "Scavenger (+50% corpse nutrition, immune to spoiled meat sickness)", 0, 0, 0, 0 },
	{ 'B', "Acid Slime",       "Floors 3-13+",     8, 2,  38,  320, 1, 0, "Acid Jet",       5, 45, "Sprays acid in 2 tiles (4 dmg, 3t poison) & spawns Acid Puddles (=).", "Caustic Body (Reflects 1 dmg), Poison/Rot Immune, Absorbs '=', No Gear", 0, 0, 0, 0 },
	{ 'G', "Ghost",            "Floors 7-13+",     4, 3,  22,  120, 2, 0, "Shadow Blink",   7, 50, "Teleports 3 tiles across walls or floor along a cardinal direction.", "Incorporeal (Phases walls/doors, sees through walls), 25% Evasion, No Gear", 0, 0, 0, 0 },
	{ 'L', "Necromancer Lich", "Floors 5-13+",    12, 6,  54,  550, 1, 1, "Soul Harvest",   6, 40, "Drains 5 necrotic HP from all foes within 3 tiles (+3 HP heal/foe).", "Soul Siphon (+2 HP melee drain), Undead Mage (15% Eva, Poison/Rot Immune)", 0, 0, 0, 0 },
	{ 'T', "Troll",            "Floors 7-13+",    15, 5,  66,  700, 3, 1, "Ground Slam",    8, 60, "Shockwave crushes all foes within 2 tiles for 7 damage.", "Regeneration (+1 HP/2t), Crushing Cleave (35% +3 dmg), Gluttony (3/t)", 0, 0, 0, 0 },
	{ 'D', "Ancient Dragon",   "Floor 10 Boss",   45, 8, 186, 1000, 2, 1, "Dragon Breath",  6, 50, "Unleashes an inferno within 3 tiles for 12 fire dmg + 3t burn.", "Apex Predator (50% Cleave, 25% Double-Hit, Fire/Acid Shield, Regen)", 0, 0, 0, 0 }
};

void RecordBestiaryEvent(char symbol, int event_type)
{
	for (int i = 0; i < BESTIARY_COUNT; i++)
	{
		if (g_bestiary[i].symbol == symbol)
		{
			g_bestiary[i].seen = 1;
			if (event_type == 1)
			{
				g_bestiary[i].kills++;
			}
			else if (event_type == 2)
			{
				g_bestiary[i].devoured++;
			}
#ifdef __EMSCRIPTEN__
			if (event_type == 0)
			{
				if (!g_bestiary[i].js_seen_synced)
				{
					g_bestiary[i].js_seen_synced = 1;
					web_unlock_bestiary(symbol, 0);
				}
			}
			else
			{
				g_bestiary[i].js_seen_synced = 1;
				web_unlock_bestiary(symbol, event_type);
			}
#endif
			break;
		}
	}
}

int ShowBestiary(Level *leveli, Level level, Entity *player)
{
	int selected = 0;
	for (int i = 0; i < BESTIARY_COUNT; i++)
	{
		if (player != NULL && g_bestiary[i].symbol == player->symbol)
		{
			selected = i;
			break;
		}
	}

	while (1)
	{
#ifdef __EMSCRIPTEN__
		for (int i = 0; i < BESTIARY_COUNT; i++)
		{
			int s = web_get_bestiary_stat(g_bestiary[i].symbol, 0);
			int k = web_get_bestiary_stat(g_bestiary[i].symbol, 1);
			int d = web_get_bestiary_stat(g_bestiary[i].symbol, 2);
			if (s > g_bestiary[i].seen) g_bestiary[i].seen = s;
			if (k > g_bestiary[i].kills) g_bestiary[i].kills = k;
			if (d > g_bestiary[i].devoured) g_bestiary[i].devoured = d;
		}
#endif
		int total_seen = 0;
		int total_devoured = 0;
		int total_kills = 0;
		for (int i = 0; i < BESTIARY_COUNT; i++)
		{
			if (g_bestiary[i].seen) total_seen++;
			if (i > 0 && g_bestiary[i].devoured > 0) total_devoured++;
			total_kills += g_bestiary[i].kills;
		}

		clear();
		mvprintw(1, 2, "==========================================================================");
		mvprintw(2, 2, "   BESTIARY & FORM COMPENDIUM  |  Sighted: %d/10  |  Forms Devoured: %d/9", total_seen, total_devoured);
		mvprintw(3, 2, "==========================================================================");

		for (int i = 0; i < BESTIARY_COUNT; i++)
		{
			BestiaryEntry *e = &g_bestiary[i];
			char p1 = (i == selected) ? '-' : ' ';
			char p2 = (i == selected) ? '>' : ' ';
			const char *status_tag = "[UNSEEN]";
			if (e->symbol == '@') status_tag = "[ORIGIN]";
			else if (e->devoured > 0) status_tag = "[DEVOURED]";
			else if (e->seen > 0) status_tag = "[SIGHTED]";

			const char *cur_tag = (player != NULL && player->symbol == e->symbol) ? " <-- CURRENT FORM" : "";
			if (e->seen || e->devoured > 0 || e->symbol == '@')
			{
				mvprintw(5 + i, 2, "%c%c [%d] (%c) %-17s %-11s | Slain: %-3d | Devoured: %-3d%s",
					p1, p2, (i + 1) % 10, e->symbol, e->name, status_tag, e->kills, e->devoured, cur_tag);
			}
			else
			{
				mvprintw(5 + i, 2, "%c%c [%d] (%c) ??? Unknown Species %-11s | Habitat: %s",
					p1, p2, (i + 1) % 10, e->symbol, status_tag, e->floors);
			}
		}

		BestiaryEntry *sel = &g_bestiary[selected];
		int rank_bonus = (player != NULL) ? (player->xp / 20) : 0;
		mvprintw(16, 2, "--------------------------------------------------------------------------");
		if (sel->seen || sel->devoured > 0 || sel->symbol == '@')
		{
			mvprintw(17, 2, "SPECIES: %s (%c)  |  Habitat: %s  |  Gear/Hands: %s",
				sel->name, sel->symbol, sel->floors, sel->can_gear ? "YES (Wields Weapons/Armor)" : "NO (Natural Beast)");
			if (sel->symbol == '@')
			{
				mvprintw(18, 2, "STATS:   Base 20 HP, 1 ATK (With Your Rk %d: %d Max HP, %d Base ATK) | Burn: 1 hunger/t",
					rank_bonus + 1, sel->poly_hp + (rank_bonus * 3), sel->base_att + rank_bonus);
			}
			else
			{
				mvprintw(18, 2, "ENEMY:   %d HP, %d ATK  |  POLYMORPH FORM: %d Max HP (%d w/Rk), %d ATK (%d w/Rk)",
					sel->base_hp, sel->base_att, sel->poly_hp, sel->poly_hp + (rank_bonus * 3),
					sel->base_att, sel->base_att + rank_bonus);
				mvprintw(19, 2, "CORPSE:  %d Base Nutrition  |  Metabolism Burn: %d hunger/turn",
					sel->nutrition, sel->hunger_rate);
			}
			mvprintw(20, 2, "TRAITS:  %s", sel->trait_desc);
			mvprintw(21, 2, "ABILITY: [%s] ('f' key | CD: %dt | Cost: %d hunger) -> %s",
				sel->ability_name, sel->ability_cd, sel->ability_cost, sel->ability_desc);
		}
		else
		{
			mvprintw(17, 2, "SPECIES: ??? (%c) [UNDISCOVERED]  |  Habitat: %s", sel->symbol, sel->floors);
			mvprintw(18, 2, "Encounter this creature in the dungeon (or devour its corpse) to record its");
			mvprintw(19, 2, "metabolic attributes, polymorph stats, passive traits, and active ability.");
		}
		mvprintw(23, 2, "Controls: [Up/Down / w/s / 1-0]: Select Species  |  ['B' / 'q' / Esc]: Return to Game");
		refresh();

		int ch = getch();
		if (ch == 'B' || ch == 'b' || ch == 'q' || ch == 'Q' || ch == 'c' || ch == 'C' || ch == 'i' || ch == 'I' || ch == 27 || ch == ' ' || ch == 10 || ch == 13)
		{
			break;
		}
		else if (ch == 'w' || ch == 'W' || ch == 'k' || ch == 'K' || ch == '8')
		{
			selected--;
			if (selected < 0) selected = BESTIARY_COUNT - 1;
		}
		else if (ch == 's' || ch == 'S' || ch == 'j' || ch == 'J' || ch == '2')
		{
			selected++;
			if (selected >= BESTIARY_COUNT) selected = 0;
		}
		else if (ch >= '1' && ch <= '9')
		{
			selected = ch - '1';
		}
		else if (ch == '0')
		{
			selected = 9;
		}
	}

	ClearMsg();
	UpdateFOV(&leveli[leveli->lvl]);
	RenderFOV(&leveli[leveli->lvl]);
	PlayerInfoBar(leveli);
	move(player->Position.yPos, player->Position.xPos);
	refresh();
	return 0;
}

int LookMode(Level *leveli, Level level, Entity *player)
{
	Level *cur_lvl = &leveli[leveli->lvl];
	int cy = player->Position.yPos;
	int cx = player->Position.xPos;

	while (1)
	{
		RenderFOV(cur_lvl);
		PlayerInfoBar(leveli);

		char desc[200] = "";
		int is_vis = (cur_lvl->visible != NULL && cy >= 0 && cy < MAX_Y && cx >= 0 && cx < MAX_X) ? cur_lvl->visible[cy][cx] : 0;
		int is_disc = (cur_lvl->discovered != NULL && cy >= 0 && cy < MAX_Y && cx >= 0 && cx < MAX_X) ? cur_lvl->discovered[cy][cx] : 0;

		if (cy == player->Position.yPos && cx == player->Position.xPos)
		{
			Item *it = GetItemAt(*cur_lvl, cy, cx);
			Corpse *corp = GetCorpseAt(*cur_lvl, cy, cx);
			if (it != NULL && it->active == YES)
			{
				if (it->item_type == ITEM_WEAPON)
					snprintf(desc, sizeof(desc), "You (%s) standing on %s (+%d ATK weapon, 'g' to pick up)", player->name, it->name, it->bonus_att);
				else
					snprintf(desc, sizeof(desc), "You (%s) standing on %s (+%d DEF armor, 'g' to pick up)", player->name, it->name, it->bonus_def);
			}
			else if (corp != NULL && corp->active == YES)
			{
				snprintf(desc, sizeof(desc), "You (%s) standing on %s %s corpse (%d nut, 'e' to eat, 'g' to stow)", player->name, GetCorpseStage(corp->age), corp->name, corp->nutrition);
			}
			else
			{
				snprintf(desc, sizeof(desc), "You: %s (%c) | HP: %d/%d | ATK: %d | DEF: %d | Trait: %s",
					player->name, leveli->player_symbol, player->health, player->maxhp,
					GetPlayerAttack(player), GetPlayerDefense(player), player->trait_desc);
			}
		}
		else if (!is_vis && !is_disc)
		{
			snprintf(desc, sizeof(desc), "Unexplored darkness (Fog of War)");
		}
		else
		{
			Entity *mon = is_vis ? GetMonsterAt(cy, cx, *cur_lvl) : NULL;
			Corpse *corp = is_vis ? GetCorpseAt(*cur_lvl, cy, cx) : NULL;
			Item *it = is_vis ? GetItemAt(*cur_lvl, cy, cx) : NULL;

			if (mon != NULL && mon->alive == YES)
			{
				const char *beh = (mon->type == FOLLOW) ? "Aggressive" : "Wandering";
				char st_buf[64] = "";
				if (mon->poison_turns > 0 && mon->snare_turns > 0)
					snprintf(st_buf, sizeof(st_buf), " [POISONED:%dt, ENSNARED:%dt]", mon->poison_turns, mon->snare_turns);
				else if (mon->poison_turns > 0)
					snprintf(st_buf, sizeof(st_buf), " [POISONED:%dt]", mon->poison_turns);
				else if (mon->snare_turns > 0)
					snprintf(st_buf, sizeof(st_buf), " [ENSNARED:%dt]", mon->snare_turns);
				snprintf(desc, sizeof(desc), "Monster: %s (%c)%s | HP: %d/%d | ATK: %d | %s | Trait: %s",
					mon->name, mon->symbol, st_buf, mon->health, mon->maxhp, mon->attack, beh, mon->trait_desc);
			}
			else if (corp != NULL && corp->active == YES)
			{
				const char *st = GetCorpseStage(corp->age);
				snprintf(desc, sizeof(desc), "Corpse: %s %s (%%) | Nutrition: %d | Polymorphs into: %s (HP:%d, ATK:%d)",
					st, corp->name, corp->nutrition, corp->name, (corp->maxhp * 4) + 6, corp->attack);
			}
			else if (it != NULL && it->active == YES)
			{
				if (it->item_type == ITEM_WEAPON)
				{
					snprintf(desc, sizeof(desc), "Weapon: %s ()) | +%d Attack Bonus (Stand here & press 'g' to pick up)",
						it->name, it->bonus_att);
				}
				else
				{
					snprintf(desc, sizeof(desc), "Armor: %s ([) | +%d Defense Bonus (Stand here & press 'g' to pick up)",
						it->name, it->bonus_def);
				}
			}
			else
			{
				char tile = (cur_lvl->map_backup != NULL) ? cur_lvl->map_backup[cy][cx] : ' ';
				const char *vis_tag = is_vis ? "" : " (Memory)";
				switch (tile)
				{
					case '.': snprintf(desc, sizeof(desc), "Stone Chamber Floor (.)%s", vis_tag); break;
					case '#': snprintf(desc, sizeof(desc), "Dark Tunnel Corridor (#)%s", vis_tag); break;
					case '+': snprintf(desc, sizeof(desc), "Closed Wooden Door (+)%s - Bump to open", vis_tag); break;
					case ':': snprintf(desc, sizeof(desc), "Open Doorway (:)%s - Press 'c' nearby to close", vis_tag); break;
					case '<': snprintf(desc, sizeof(desc), "Downstairs (<)%s - Leads deeper into the dungeon", vis_tag); break;
					case '>': snprintf(desc, sizeof(desc), "Upstairs (>)%s - Leads toward the surface", vis_tag); break;
					case '*': snprintf(desc, sizeof(desc), "Shimmering Golden Portal (*)%s - Step here to escape & win!", vis_tag); break;
					case '_': snprintf(desc, sizeof(desc), "Metabolic Altar (_)%s - Purifies tainted corpses, heals +5 HP & grants +10 XP", vis_tag); break;
					case '~': snprintf(desc, sizeof(desc), "Subterranean Spring (~)%s - Restores +450 hunger, +10 HP & resets ability CD", vis_tag); break;
					case '^': snprintf(desc, sizeof(desc), "Sticky Spider Web (^)%s - Ensnares foes for 3t or drains 20 player hunger (Spiders/Ghosts ignore)", vis_tag); break;
					case '=': snprintf(desc, sizeof(desc), "Caustic Acid Puddle (=)%s - Burns for acid dmg & poison (Slimes absorb for +2 HP & +30 hunger)", vis_tag); break;
					case '-':
					case '|': snprintf(desc, sizeof(desc), "Solid Dungeon Wall (%c)%s", tile, vis_tag); break;
					default:  snprintf(desc, sizeof(desc), "Empty Void%s", vis_tag); break;
				}
			}
		}

		// Draw Look Cursor 'X' at (cy, cx) and place terminal cursor there so camera pans to it:
		PrintChar(cy, cx, 'X');
		ClearMsg();
		mvprintw(0, 0, "LOOK [Move: pan | v/c/Esc/Space: exit] >> %s", desc);
		move(cy, cx);
		refresh();

		int ch = getch();

		// Exit Look Mode:
		if (ch == 'v' || ch == 'V' || ch == ';' || ch == '/' || ch == '?' ||
		    ch == 'c' || ch == 'C' || ch == 'i' || ch == 'I' || ch == ' ' ||
		    ch == 10 || ch == 13 || ch == 27)
		{
			break;
		}

		int ny = cy;
		int nx = cx;
		switch (ch)
		{
			case 'w': case 'W': case 'k': case 'K': case '8': ny--; break;
			case 's': case 'S': case 'j': case 'J': case '2': ny++; break;
			case 'a': case 'A': case 'h': case 'H': case '4': nx--; break;
			case 'd': case 'D': case 'l': case 'L': case '6': nx++; break;
			case 'q': case 'Q': case 'y': case 'Y': case '7': ny--; nx--; break;
			case 'e': case 'E': case 'u': case 'U': case '9': ny--; nx++; break;
			case 'z': case 'Z': case 'b': case 'B': case '1': ny++; nx--; break;
			case 'x': case 'X': case 'n': case 'N': case '3': ny++; nx++; break;
			default: break;
		}

		if (ny >= 1 && ny < MAX_Y && nx >= 0 && nx < MAX_X)
		{
			cy = ny;
			cx = nx;
		}
	}

	ClearMsg();
	UpdateFOV(cur_lvl);
	RenderFOV(cur_lvl);
	PlayerInfoBar(leveli);
	move(player->Position.yPos, player->Position.xPos);
	refresh();
	return 0;
}

int QuickWield(Level *leveli, Level level, Entity *player)
{
	if(player->can_open_doors == NO)
	{
		ClearMsg();
		mvprintw(0, 0, "Your %s body lacks hands to wield weapons!", player->name);
		refresh();
		return 0;
	}

	ClearMsg();
	mvprintw(0, 0, "Wield which item? (Press 'a'-'%c' or Space to cancel)", 'a' + player->num_items - 1);
	refresh();
	int ch = getch();
	if(ch >= 'a' && ch < ('a' + player->num_items))
	{
		int idx = ch - 'a';
		if(player->inventory[idx].item_type == ITEM_WEAPON)
		{
			if(player->equipped_weapon >= 0)
			{
				player->inventory[player->equipped_weapon].equipped = NO;
			}
			player->inventory[idx].equipped = YES;
			player->equipped_weapon = idx;
			ClearMsg();
			mvprintw(0, 0, "Wielded %s (+%d ATK)!", player->inventory[idx].name, player->inventory[idx].bonus_att);
			PlayerInfoBar(leveli);
			refresh();
			return 1;
		}
		else if(player->inventory[idx].item_type == ITEM_ARMOR)
		{
			if(player->equipped_armor >= 0)
			{
				player->inventory[player->equipped_armor].equipped = NO;
			}
			player->inventory[idx].equipped = YES;
			player->equipped_armor = idx;
			ClearMsg();
			mvprintw(0, 0, "Equipped %s (+%d DEF)!", player->inventory[idx].name, player->inventory[idx].bonus_def);
			PlayerInfoBar(leveli);
			refresh();
			return 1;
		}
	}
	ClearMsg();
	return 0;
}

int QuickDrop(Level *leveli, Level level, Entity *player)
{
	int py = player->Position.yPos;
	int px = player->Position.xPos;

	if(player->num_items == 0)
	{
		ClearMsg();
		mvprintw(0, 0, "Your backpack is empty.");
		refresh();
		return 0;
	}

	ClearMsg();
	mvprintw(0, 0, "Drop which item? (Press 'a'-'%c' or Space to cancel)", 'a' + player->num_items - 1);
	refresh();
	int ch = getch();
	if(ch >= 'a' && ch < ('a' + player->num_items))
	{
		int idx = ch - 'a';
		Item it = player->inventory[idx];

		// If dropping an equipped item, unequip it
		if(idx == player->equipped_weapon) player->equipped_weapon = -1;
		else if(player->equipped_weapon > idx) player->equipped_weapon--;

		if(idx == player->equipped_armor) player->equipped_armor = -1;
		else if(player->equipped_armor > idx) player->equipped_armor--;

		// Remove from inventory
		for(int k = idx; k < player->num_items - 1; k++)
		{
			player->inventory[k] = player->inventory[k + 1];
		}
		player->num_items--;

		// If corpse, drop as corpse retaining elapsed age
		if(it.item_type == ITEM_CORPSE)
		{
			Entity dummy;
			dummy.mode = 0;
			dummy.symbol = it.corpse_data.symbol;
			dummy.name = it.corpse_data.name;
			dummy.maxhp = it.corpse_data.maxhp;
			dummy.attack = it.corpse_data.attack;
			DropCorpse(leveli, &dummy, py, px);
			int c_idx = leveli[leveli->lvl].num_corpses - 1;
			if(c_idx >= 0)
			{
				leveli[leveli->lvl].corpses[c_idx].age = it.corpse_data.age;
				leveli[leveli->lvl].corpses[c_idx].nutrition = it.corpse_data.nutrition;
			}
			player->Position.ch = leveli[leveli->lvl].map_backup[py][px];
			ClearMsg();
			mvprintw(0, 0, "Dropped %s %s corpse on the floor.", GetCorpseStage(it.corpse_data.age), it.name);
		}
		else
		{
			// Add to floor items
			int lidx = leveli[leveli->lvl].num_items;
			if(lidx < leveli[leveli->lvl].max_items)
			{
				leveli[leveli->lvl].items[lidx] = it;
				leveli[leveli->lvl].items[lidx].Position.yPos = py;
				leveli[leveli->lvl].items[lidx].Position.xPos = px;
				leveli[leveli->lvl].items[lidx].active = YES;
				leveli[leveli->lvl].items[lidx].equipped = NO;
				leveli[leveli->lvl].num_items++;
			}
			char cur_tile = leveli[leveli->lvl].map_backup[py][px];
			if(cur_tile != '<' && cur_tile != '>' && cur_tile != ':' && cur_tile != '+' && cur_tile != '*' && cur_tile != '_' && cur_tile != '~')
			{
				leveli[leveli->lvl].map_backup[py][px] = it.symbol;
			}
			player->Position.ch = leveli[leveli->lvl].map_backup[py][px];
			ClearMsg();
			mvprintw(0, 0, "Dropped %s on the floor.", it.name);
		}
		PlayerInfoBar(leveli);
		refresh();
		return 1;
	}
	ClearMsg();
	return 0;
}

int EatFromInventory(Level *leveli, Level level, Entity *player)
{
	int corpses[10];
	int count = 0;
	for(int i = 0; i < player->num_items; i++)
	{
		if(player->inventory[i].item_type == ITEM_CORPSE)
		{
			corpses[count++] = i;
		}
	}

	if(count == 0)
	{
		ClearMsg();
		mvprintw(0, 0, "No corpses stored in your backpack.");
		refresh();
		return 0;
	}

	ClearMsg();
	mvprintw(0, 0, "Eat stored corpse: ");
	for(int i = 0; i < count; i++)
	{
		int idx = corpses[i];
		printw("[%c] %s  ", 'a' + idx, player->inventory[idx].name);
	}
	printw("(or Space to cancel)");
	refresh();

	int ch = getch();
	if(ch >= 'a' && ch < ('a' + player->num_items))
	{
		int idx = ch - 'a';
		if(player->inventory[idx].item_type == ITEM_CORPSE)
		{
			Corpse saved = player->inventory[idx].corpse_data;
			if(player->equipped_weapon > idx) player->equipped_weapon--;
			if(player->equipped_armor > idx) player->equipped_armor--;
			for(int k = idx; k < player->num_items - 1; k++)
			{
				player->inventory[k] = player->inventory[k + 1];
			}
			player->num_items--;
			EatCorpse(leveli, level, player, &saved);
			return 1;
		}
	}
	ClearMsg();
	return 0;
}

int ExitScreen(Level *level)
{
	Entity *pl = level[level->lvl].player;
	int is_victory = (level->killer != NULL && strcmp(level->killer, "VICTORY") == 0);
	int total_att = GetPlayerAttack(pl);
	int total_def = GetPlayerDefense(pl);
	int rank = (pl->xp / 20) + 1;
	int score = pl->xp * 10 + level->maxlvl * 50 + (is_victory ? 1000 : 0);

	PlaySound(is_victory ? "victory" : "gameover");

#ifdef __EMSCRIPTEN__
	web_record_run(is_victory, pl->name, level->player_symbol,
		level->killer ? level->killer : "Quit",
		pl->xp, rank, level->maxlvl, level->turn,
		pl->health, pl->maxhp, total_att, total_def);
#endif

	clear();

	if(is_victory)
	{
		mvprintw(7, 10, "================================================================");
		mvprintw(8, 10, "        * * *  GLORIOUS VICTORY - DUNGEON CONQUERED!  * * *     ");
		mvprintw(9, 10, "================================================================");
		mvprintw(11, 10, "You slew the Ancient Dragon of Floor 10 and stepped through");
		mvprintw(12, 10, "the shimmering Golden Portal (*) back into the sunlight!");
		mvprintw(14, 10, "Final Form:     %s (%c)", pl->name, level->player_symbol);
		mvprintw(15, 10, "Final Stats:    %d/%d HP | %d ATK | %d DEF", pl->health, pl->maxhp, total_att, total_def);
		mvprintw(16, 10, "Deepest Floor:  Floor %d", level->maxlvl);
		mvprintw(17, 10, "Turns Taken:    %d", level->turn);
		mvprintw(18, 10, "Mastery Rank:   Rank %d (%d XP)", rank, pl->xp);
		mvprintw(19, 10, "Final Score:    %d pts", score);
		mvprintw(20, 10, "Dungeon Seed:   %s (%s)",
			GAME_SEED_LABEL,
			GAME_SEED_MODE == 1 ? "Daily Challenge" : (GAME_SEED_MODE == 2 ? "Custom Seed" : "Random"));
		mvprintw(22, 10, "[Run recorded to Hall of Fame! Click 'Fame' in top bar to view]");
		mvprintw(24, 10, ">> Press any key to conclude your legend <<");
		refresh();
		getch();
	}
	else
	{
		mvprintw(8, 10, "================================================================");
		mvprintw(9, 10, "                     * * *  GAME OVER!  * * *                   ");
		mvprintw(10, 10, "================================================================");
		mvprintw(12, 10, "You perished as a %s (%c), slain by: %s",
			pl->name, level->player_symbol, level->killer ? level->killer : "Unknown");
		mvprintw(14, 10, "Final Stats:    %d/%d HP | %d ATK | %d DEF", pl->health, pl->maxhp, total_att, total_def);
		mvprintw(15, 10, "Deepest Floor:  Floor %d", level->maxlvl);
		mvprintw(16, 10, "Turns Taken:    %d", level->turn);
		mvprintw(17, 10, "Mastery Rank:   Rank %d (%d XP)", rank, pl->xp);
		mvprintw(18, 10, "Final Score:    %d pts", score);
		mvprintw(19, 10, "Dungeon Seed:   %s (%s)",
			GAME_SEED_LABEL,
			GAME_SEED_MODE == 1 ? "Daily Challenge" : (GAME_SEED_MODE == 2 ? "Custom Seed" : "Random"));
		mvprintw(21, 10, "[Run recorded to Hall of Fame! Click 'Fame' in top bar to view]");
		mvprintw(23, 10, ">> Press any key to continue <<");
		refresh();
		getch();
	}
	
	clear();
	mvprintw(27,76,"THANK YOU FOR PLAYING!");
	refresh(); getch();

	return 1;
}

int PrintChar(int y, int x, char c)
{
	mvprintw(y, x, "%c", c);
	return 1;
}

int PrintNum(int num, char *c)
{
	char buf[8];
	
	int len = 1; len = strlen(c);
	
	ClearMsg();
	mvprintw(0,0,"%s",c);
	sprintf(buf,"%d",num);	
	mvprintw(0,(len-3),"%s",buf);	
	refresh();
	getch();
	
	return 1;
}

int PrintMap(int y, int x, char **map)
{
	clear();
	for(int i=0; i<y; i++)
	{
		for(int j=0; j<x; j++)
		{
			PrintChar(i,j,map[i][j]);
		}
	}
	return 1;
}

int AskStairs(int stairs, Entity *player)
{
	switch(stairs)
	{
	case DSTAIRS:
		ClearMsg();
		mvprintw(0,0,"You see downstairs here, would you like to enter? y/n");	
		refresh();
		player->input = getch();
		ClearMsg();

		if(player->input=='y' || player->input=='Y')
		{
			player->Position.type=DSTAIRS;		
		} 
	break;
	
	case USTAIRS:
		ClearMsg();
		mvprintw(0,0,"You see upstairs here, would you like to enter? y/n");	
		refresh();
		player->input = getch();
		ClearMsg();
		if(player->input=='y' || player->input=='Y')
		{
			player->Position.type=USTAIRS;		
		} 
	break;

	default:
		ClearMsg();
		mvprintw(0,0,"ERROR!!!!");	
		refresh();
		ClearMsg();
	break;
	}

	return 1;
}

int OpenDoor(Level *leveli, Entity *player, int y, int x)
{
	ClearMsg();
	mvprintw(0,0, "Do you want to open the door? y/n");
	refresh();
	player->input = getch();
	ClearMsg();
	if(player->input=='y' || player->input=='Y')
	{
		PlaySound("door");
		mvprintw(0,0, "You opened the door! (close a door with 'c')");
		if(leveli != NULL && leveli[leveli->lvl].map_backup != NULL)
		{
			leveli[leveli->lvl].map_backup[y][x] = ':';
		}
		PrintChar(y,x,':');
		refresh();
		move(player->Position.yPos,player->Position.xPos);
		return 1;
	} 
	else
	{
		move(player->Position.yPos,player->Position.xPos);
		refresh();
		return 0;
	}
}

