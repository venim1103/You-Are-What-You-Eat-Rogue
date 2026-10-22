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
	mvprintw(0,0, "Move: wasd/qezx/numpad | Ability: 'f' | Wait: '.' | Eat: 'e'/'E' | Close: 'c' | Quit: 'Q'");
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
	printw("    XP: %d", level[level->lvl].player->xp);
	printw("      ");

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

		// Only place on free floor:
		if((mvinch(y, x) & A_CHARTEXT) != '.')
		{
			continue;
		}

		int idx = level->num_items;
		level->items[idx].Position.yPos = y;
		level->items[idx].Position.xPos = x;
		level->items[idx].active = YES;
		level->items[idx].equipped = NO;

		int roll = rand() % 7;
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
		else
		{
			level->items[idx].symbol = '[';
			level->items[idx].name = "Steel Plate Armor";
			level->items[idx].bonus_att = 0;
			level->items[idx].bonus_def = 6;
			level->items[idx].item_type = ITEM_ARMOR;
		}

		level->map_backup[y][x] = level->items[idx].symbol;
		PrintChar(y, x, level->items[idx].symbol);
		level->num_items++;
	}

	free(yx);
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
		leveli[leveli->lvl].map_backup[py][px] = '.';
		player->Position.ch = '.';

		ClearMsg();
		if(it->item_type == ITEM_WEAPON)
		{
			mvprintw(0, 0, "Picked up %s (+%d ATK)! Press 'w' to wield or 'i' for pack.", it->name, it->bonus_att);
		}
		else
		{
			mvprintw(0, 0, "Picked up %s (+%d DEF)! Press 'w' to wear or 'i' for pack.", it->name, it->bonus_def);
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
		leveli[leveli->lvl].map_backup[py][px] = '.';
		player->Position.ch = '.';

		ClearMsg();
		mvprintw(0, 0, "Stowed %s corpse into backpack! (Slot %c) Press 'E' to eat anytime.", c->name, 'a' + slot);
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

	while (1)
	{
		clear();
		mvprintw(1, 2, "==========================================================");
		mvprintw(2, 2, "   BACKPACK & INVENTORY SLOTS (10 Slots Total)");
		mvprintw(3, 2, "==========================================================");

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
				mvprintw(5 + i, 2, "%c%c [Slot %2d] <Empty>", p1, p2, i + 1);
			}
		}

		mvprintw(16, 2, "----------------------------------------------------------");
		mvprintw(17, 2, "Current Form: %s (%c) | Hands: %s", player->name, player->symbol, (player->can_open_doors == YES ? "YES (Can wield/wear)" : "NO (Beast - cannot wield)"));
		mvprintw(18, 2, "Controls:");
		mvprintw(19, 4, "[Up/Down / w/s / 8/2]: Move selection (Current: Slot %d)", selected + 1);
		mvprintw(20, 4, "[Enter / Space / 'e' / 'f']: Use/Equip/Eat selected item");
		mvprintw(21, 4, "['g' / 'd' / Pick]: Drop selected item to floor");
		mvprintw(22, 4, "['q' / 'i' / 'c' / Esc]: Exit backpack");
		refresh();

		int ch = getch();

		// Navigate Up:
		if (ch == 'w' || ch == 'W' || ch == 'k' || ch == 'K' || ch == '8')
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
		// Letter keys 'a'-'j':
		else if (ch >= 'a' && ch <= 'j')
		{
			selected = ch - 'a';
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
					leveli[leveli->lvl].map_backup[py][px] = it.symbol;
				}
				ClearMsg();
				mvprintw(0, 0, "Dropped %s on the floor.", it.name);
				refresh();
			}
			else
			{
				ClearMsg();
				mvprintw(0, 0, "Slot %d is empty, nothing to drop.", selected + 1);
				refresh();
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
						ClearMsg();
						mvprintw(0, 0, "Your %s body lacks hands to wield weapons!", player->name);
						refresh();
					}
					else
					{
						if (it->equipped == YES)
						{
							it->equipped = NO;
							player->equipped_weapon = -1;
							ClearMsg();
							mvprintw(0, 0, "Sheathed %s.", it->name);
						}
						else
						{
							if (player->equipped_weapon >= 0)
							{
								player->inventory[player->equipped_weapon].equipped = NO;
							}
							it->equipped = YES;
							player->equipped_weapon = selected;
							ClearMsg();
							mvprintw(0, 0, "Wielded %s (+%d ATK)!", it->name, it->bonus_att);
						}
						refresh();
					}
				}
				else if (it->item_type == ITEM_ARMOR)
				{
					if (player->can_open_doors == NO)
					{
						ClearMsg();
						mvprintw(0, 0, "Your %s body lacks hands to wear armor!", player->name);
						refresh();
					}
					else
					{
						if (it->equipped == YES)
						{
							it->equipped = NO;
							player->equipped_armor = -1;
							ClearMsg();
							mvprintw(0, 0, "Took off %s.", it->name);
						}
						else
						{
							if (player->equipped_armor >= 0)
							{
								player->inventory[player->equipped_armor].equipped = NO;
							}
							it->equipped = YES;
							player->equipped_armor = selected;
							ClearMsg();
							mvprintw(0, 0, "Equipped %s (+%d DEF)!", it->name, it->bonus_def);
						}
						refresh();
					}
				}
				else if (it->item_type == ITEM_CORPSE)
				{
					Corpse saved_corpse = it->corpse_data;
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
				ClearMsg();
				mvprintw(0, 0, "Slot %d is empty.", selected + 1);
				refresh();
			}
		}
		// EXIT INVENTORY: 'q', 'Q', 'i', 'I', 'c', 'C', 27 (Esc)
		else if (ch == 'q' || ch == 'Q' || ch == 'i' || ch == 'I' || ch == 'c' || ch == 'C' || ch == 27)
		{
			break;
		}
	}

	UpdateFOV(&leveli[leveli->lvl]);
	RenderFOV(&leveli[leveli->lvl]);
	PlayerInfoBar(leveli);
	refresh();
	return 1;
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

		// If corpse, drop as corpse
		if(it.item_type == ITEM_CORPSE)
		{
			Entity dummy;
			dummy.symbol = it.corpse_data.symbol;
			dummy.name = it.corpse_data.name;
			dummy.maxhp = it.corpse_data.maxhp;
			dummy.attack = it.corpse_data.attack;
			DropCorpse(leveli, &dummy, py, px);
			ClearMsg();
			mvprintw(0, 0, "Dropped %s corpse on the floor.", it.name);
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
			leveli[leveli->lvl].map_backup[py][px] = it.symbol;
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
	char buf[8];
	sprintf(buf,"%d",level[level->lvl].player->xp);
	clear();

	mvprintw(25,0,"                                                               YOU WERE A %s KILLED BY A %s AND DIED WITH %s XP!",
		level[level->lvl].player->name, level->killer, buf);
	printw("\n\n\n\n\n                                                                                   GAME OVER!");
	refresh(); getch();
	
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

int OpenDoor(Entity *player, int y, int x)
{
	if(player->can_open_doors == NO)
	{
		ClearMsg();
		mvprintw(0,0,"You cannot open doors as a %s! (You lack hands)", player->name);
		refresh(); getch();
		ClearMsg();
		refresh();
		move(player->Position.yPos, player->Position.xPos);
		return 0;
	}

	ClearMsg();
	mvprintw(0,0, "Do you want to open the door? y/n");
	refresh();
	player->input = getch();
	ClearMsg();
	if(player->input=='y' || player->input=='Y')
	{
		ClearMsg();
		mvprintw(0,0, "You opened the door! (close a door with 'c')");
		refresh(); getch();
		ClearMsg();
		PrintChar(y,x,':');
	} 
	else
	{
		move(player->Position.yPos,player->Position.xPos);
	}
	return 1;
}

