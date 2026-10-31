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
	mvprintw(17, 4, "Wait:    '.' or Space / [Next]");
	mvprintw(18, 4, "Close:   'c' / [Close] (doors)");
	mvprintw(20, 2, ">> Tap [ Next ] or press any key to enter <<");
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

int WelcomeTxt()
{
	// WELCOME TEXT + INSTRUCTIONS:
	ClearMsg();
	mvprintw(0,0, "Move: wasd/qezx | Look: 'v' | Pack: 'i' | Pick: 'g' | Ability: 'f' | Eat: 'E' | Quit: 'Q'");
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
	printw("      ");

	move(level[level->lvl].player->Position.yPos, level[level->lvl].player->Position.xPos);

	return 1;
}

void SpawnFloorItems(Level *level, Entity rooms, int lvl)
{
	int spawn_count = (rand() % 3) + 2; // 2 to 4 items per level
	int *yx = malloc(sizeof(int) * 3);

	for(int i = 0; i < spawn_count && level->num_items < level->max_items; i++)
	{
		int random_room = rand() % rooms.mode;
		yx = RandomCoords(yx, rooms.mode, rooms.Area[random_room]);
		int y = yx[0];
		int x = yx[1];

		// Only place on free floor (and never overwrite player/stairs/monsters/other items):
		if(level->map_backup[y][x] != '.' ||
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
		mvprintw(21, 10, "[Run recorded to Hall of Fame! Click 'Fame' in top bar to view]");
		mvprintw(23, 10, ">> Press any key to conclude your legend <<");
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
		mvprintw(20, 10, "[Run recorded to Hall of Fame! Click 'Fame' in top bar to view]");
		mvprintw(22, 10, ">> Press any key to continue <<");
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

