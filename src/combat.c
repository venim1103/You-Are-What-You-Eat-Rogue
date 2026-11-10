#include "global.h"

static const char *g_last_loot_drop = NULL;

const char *PopLastLootDrop(void)
{
	const char *res = g_last_loot_drop;
	g_last_loot_drop = NULL;
	return res;
}

const char *SpawnEliteLootDrop(Level *leveli, Entity *monster, int y, int x)
{
	if(leveli == NULL || monster == NULL || monster->is_elite <= 0) return NULL;
	int lvl = leveli->lvl;
	Level *cur = &leveli[lvl];
	if(cur->items == NULL || cur->num_items >= cur->max_items) return NULL;

	// Mini-bosses (is_elite == 2) guarantee a drop (100%); Elite variants (is_elite == 1) have a 55% chance:
	if(monster->is_elite == 1 && (rand() % 100) >= 55)
	{
		return NULL;
	}

	// Find a nearby walkable tile (prefer adjacent '.' so both corpse '%' and loot item are visible):
	int dy[9] = {0, -1, 1, 0, 0, -1, -1, 1, 1};
	int dx[9] = {0, 0, 0, -1, 1, -1, 1, -1, 1};
	int drop_y = y;
	int drop_x = x;
	for(int d = 1; d < 9; d++)
	{
		int ny = y + dy[d];
		int nx = x + dx[d];
		if(ny > 0 && ny < MAX_Y - 1 && nx > 0 && nx < MAX_X - 1 &&
		   cur->map_backup != NULL && cur->map_backup[ny][nx] == '.' &&
		   GetMonsterAt(ny, nx, *cur) == NULL &&
		   GetItemAt(*cur, ny, nx) == NULL &&
		   GetCorpseAt(*cur, ny, nx) == NULL &&
		   (cur->player == NULL || cur->player->Position.yPos != ny || cur->player->Position.xPos != nx))
		{
			drop_y = ny;
			drop_x = nx;
			break;
		}
	}

	int idx = cur->num_items;
	cur->items[idx].Position.yPos = drop_y;
	cur->items[idx].Position.xPos = drop_x;
	cur->items[idx].active = YES;
	cur->items[idx].equipped = NO;

	if(monster->is_elite == 2)
	{
		// Mini-Boss Guaranteed Relic Drop:
		if(lvl <= 4)
		{
			if((rand() % 2) == 0)
			{
				cur->items[idx].symbol = ')';
				cur->items[idx].name = "Venomfang Sabre";
				cur->items[idx].bonus_att = 6;
				cur->items[idx].bonus_def = 0;
				cur->items[idx].item_type = ITEM_WEAPON;
			}
			else
			{
				cur->items[idx].symbol = '[';
				cur->items[idx].name = "Arachnid Carapace";
				cur->items[idx].bonus_att = 0;
				cur->items[idx].bonus_def = 5;
				cur->items[idx].item_type = ITEM_ARMOR;
			}
		}
		else if(lvl <= 7)
		{
			if((rand() % 2) == 0)
			{
				cur->items[idx].symbol = ')';
				cur->items[idx].name = "Warlord's Cleaver";
				cur->items[idx].bonus_att = 9;
				cur->items[idx].bonus_def = 0;
				cur->items[idx].item_type = ITEM_WEAPON;
			}
			else
			{
				cur->items[idx].symbol = '[';
				cur->items[idx].name = "Warlord's Plate";
				cur->items[idx].bonus_att = 0;
				cur->items[idx].bonus_def = 7;
				cur->items[idx].item_type = ITEM_ARMOR;
			}
		}
		else
		{
			if((rand() % 2) == 0)
			{
				cur->items[idx].symbol = ')';
				cur->items[idx].name = "Soulreaver Scythe";
				cur->items[idx].bonus_att = 11;
				cur->items[idx].bonus_def = 0;
				cur->items[idx].item_type = ITEM_WEAPON;
			}
			else
			{
				cur->items[idx].symbol = '[';
				cur->items[idx].name = "Arch-Lich Robes";
				cur->items[idx].bonus_att = 0;
				cur->items[idx].bonus_def = 9;
				cur->items[idx].item_type = ITEM_ARMOR;
			}
		}
	}
	else
	{
		// Elite Variant Enhanced Loot Drop:
		int roll = rand() % 5;
		if(roll == 4)
		{
			int c_roll = rand() % 4;
			if(c_roll == 0) { cur->items[idx].symbol = '!'; cur->items[idx].name = "Mutagenic Serum"; cur->items[idx].bonus_att = 2; cur->items[idx].bonus_def = 0; cur->items[idx].item_type = ITEM_CONSUMABLE; }
			else if(c_roll == 1) { cur->items[idx].symbol = '?'; cur->items[idx].name = "Scroll of Discord"; cur->items[idx].bonus_att = 3; cur->items[idx].bonus_def = 0; cur->items[idx].item_type = ITEM_CONSUMABLE; }
			else if(c_roll == 2) { cur->items[idx].symbol = '?'; cur->items[idx].name = "Scroll of Torment"; cur->items[idx].bonus_att = 4; cur->items[idx].bonus_def = 0; cur->items[idx].item_type = ITEM_CONSUMABLE; }
			else { cur->items[idx].symbol = '!'; cur->items[idx].name = "Crimson Vitality Vial"; cur->items[idx].bonus_att = 1; cur->items[idx].bonus_def = 0; cur->items[idx].item_type = ITEM_CONSUMABLE; }
		}
		else if(lvl <= 3)
		{
			if(roll == 0) { cur->items[idx].symbol = ')'; cur->items[idx].name = "Steel Shortsword"; cur->items[idx].bonus_att = 4; cur->items[idx].bonus_def = 0; cur->items[idx].item_type = ITEM_WEAPON; }
			else if(roll == 1) { cur->items[idx].symbol = ')'; cur->items[idx].name = "Heavy Broadsword"; cur->items[idx].bonus_att = 6; cur->items[idx].bonus_def = 0; cur->items[idx].item_type = ITEM_WEAPON; }
			else if(roll == 2) { cur->items[idx].symbol = '['; cur->items[idx].name = "Iron Chainmail"; cur->items[idx].bonus_att = 0; cur->items[idx].bonus_def = 4; cur->items[idx].item_type = ITEM_ARMOR; }
			else { cur->items[idx].symbol = '['; cur->items[idx].name = "Arachnid Carapace"; cur->items[idx].bonus_att = 0; cur->items[idx].bonus_def = 5; cur->items[idx].item_type = ITEM_ARMOR; }
		}
		else if(lvl <= 6)
		{
			if(roll == 0) { cur->items[idx].symbol = ')'; cur->items[idx].name = "Heavy Broadsword"; cur->items[idx].bonus_att = 6; cur->items[idx].bonus_def = 0; cur->items[idx].item_type = ITEM_WEAPON; }
			else if(roll == 1) { cur->items[idx].symbol = ')'; cur->items[idx].name = "Trollish Warhammer"; cur->items[idx].bonus_att = 8; cur->items[idx].bonus_def = 0; cur->items[idx].item_type = ITEM_WEAPON; }
			else if(roll == 2) { cur->items[idx].symbol = '['; cur->items[idx].name = "Steel Plate Armor"; cur->items[idx].bonus_att = 0; cur->items[idx].bonus_def = 6; cur->items[idx].item_type = ITEM_ARMOR; }
			else { cur->items[idx].symbol = '['; cur->items[idx].name = "Warlord's Plate"; cur->items[idx].bonus_att = 0; cur->items[idx].bonus_def = 7; cur->items[idx].item_type = ITEM_ARMOR; }
		}
		else
		{
			if(roll == 0) { cur->items[idx].symbol = ')'; cur->items[idx].name = "Trollish Warhammer"; cur->items[idx].bonus_att = 8; cur->items[idx].bonus_def = 0; cur->items[idx].item_type = ITEM_WEAPON; }
			else if(roll == 1) { cur->items[idx].symbol = ')'; cur->items[idx].name = "Runed Greatsword"; cur->items[idx].bonus_att = 10; cur->items[idx].bonus_def = 0; cur->items[idx].item_type = ITEM_WEAPON; }
			else if(roll == 2) { cur->items[idx].symbol = '['; cur->items[idx].name = "Dragonscale Mail"; cur->items[idx].bonus_att = 0; cur->items[idx].bonus_def = 8; cur->items[idx].item_type = ITEM_ARMOR; }
			else { cur->items[idx].symbol = ')'; cur->items[idx].name = "Soulreaver Scythe"; cur->items[idx].bonus_att = 11; cur->items[idx].bonus_def = 0; cur->items[idx].item_type = ITEM_WEAPON; }
		}
	}

	if(cur->map_backup != NULL)
	{
		char tile = cur->map_backup[drop_y][drop_x];
		if(tile == '.')
		{
			cur->map_backup[drop_y][drop_x] = cur->items[idx].symbol;
		}
	}
	PrintChar(drop_y, drop_x, cur->items[idx].symbol);
	cur->num_items++;
	g_last_loot_drop = cur->items[idx].name;
	return cur->items[idx].name;
}

int DropCorpse(Level *leveli, Entity *monster, int y, int x)
{
	if(monster != NULL && monster->mode != 0)
	{
		RecordBestiaryEvent(monster->symbol, 1);
	}
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
		leveli[lvl].corpses[idx].is_elite = monster->is_elite;
		leveli[lvl].corpses[idx].active = YES;

		switch(monster->symbol)
		{
			case 'x': leveli[lvl].corpses[idx].nutrition = 150; break;
			case 's': leveli[lvl].corpses[idx].nutrition = 250; break;
			case 'V': leveli[lvl].corpses[idx].nutrition = 100; break;
			case 'B': leveli[lvl].corpses[idx].nutrition = 320; break;
			case 'G': leveli[lvl].corpses[idx].nutrition = 120; break;
			case 'g': leveli[lvl].corpses[idx].nutrition = 400; break;
			case 'L': leveli[lvl].corpses[idx].nutrition = 550; break;
			case 'T': leveli[lvl].corpses[idx].nutrition = 700; break;
			case 'D': leveli[lvl].corpses[idx].nutrition = 1000; break;
			default:  leveli[lvl].corpses[idx].nutrition = 300; break;
		}
		if(monster->is_elite == 1)
		{
			leveli[lvl].corpses[idx].nutrition = (leveli[lvl].corpses[idx].nutrition * 150) / 100;
		}
		else if(monster->is_elite == 2 && monster->symbol != 'D')
		{
			leveli[lvl].corpses[idx].nutrition = leveli[lvl].corpses[idx].nutrition * 2;
		}
		leveli[lvl].corpses[idx].age = 0;
		leveli[lvl].num_corpses++;
	}

	char cur_tile = leveli[lvl].map_backup[y][x];
	if(cur_tile != '<' && cur_tile != '>' && cur_tile != ':' && cur_tile != '+' && cur_tile != '*' && cur_tile != '_' && cur_tile != '~')
	{
		leveli[lvl].map_backup[y][x] = '%';
	}
	PrintChar(y, x, '%');

	if(monster != NULL && monster->mode != 0 && monster->is_elite > 0 && monster->symbol != 'D')
	{
		SpawnEliteLootDrop(leveli, monster, y, x);
	}

	// If the slain monster is the Ancient Dragon boss, spawn the Golden Portal (*) nearby:
	if(monster->symbol == 'D')
	{
		int dy[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
		int dx[8] = {0, 0, -1, 1, -1, 1, -1, 1};
		int port_y = y;
		int port_x = x;
		for(int d = 0; d < 8; d++)
		{
			int ny = y + dy[d];
			int nx = x + dx[d];
			if(ny > 0 && ny < MAX_Y - 1 && nx > 0 && nx < MAX_X - 1 &&
			   leveli[lvl].map_backup[ny][nx] == '.' &&
			   (leveli[lvl].player == NULL ||
			    leveli[lvl].player->Position.yPos != ny ||
			    leveli[lvl].player->Position.xPos != nx))
			{
				port_y = ny;
				port_x = nx;
				break;
			}
		}
		leveli[lvl].map_backup[port_y][port_x] = '*';
		if(leveli[lvl].discovered != NULL) leveli[lvl].discovered[port_y][port_x] = 1;
		if(leveli[lvl].visible != NULL) leveli[lvl].visible[port_y][port_x] = 1;
		PrintChar(port_y, port_x, '*');
	}

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
		// 50% chance of food poisoning unless player has scavenger bonus or acid/poison/undead immunity
		if(player->scavenger_bonus == 0 && player->symbol != 'B' && player->symbol != 's' && player->symbol != 'L')
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
		if(player->scavenger_bonus == 0 && player->symbol != 'B' && player->symbol != 'L')
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

	int rank_bonus = player->xp / 20;
	player->name = corpse->name;
	player->symbol = corpse->symbol;
	leveli->player_symbol = corpse->symbol;
	player->attack = corpse->attack + rank_bonus;
	player->maxhp = ((corpse->maxhp)*4)+6 + (rank_bonus * 3);
	player->health = player->maxhp;

	player->hunger += nut;
	if(player->hunger > player->max_hunger)
	{
		player->hunger = player->max_hunger;
	}

	ApplyFormTraits(player, corpse->symbol);
	RecordBestiaryEvent(corpse->symbol, 2);

	// If newly polymorphed form lacks hands, auto-sheathe weapons and armor:
	if(player->can_open_doors == NO)
	{
		if(player->equipped_weapon >= 0)
		{
			player->inventory[player->equipped_weapon].equipped = NO;
			player->equipped_weapon = -1;
		}
		if(player->equipped_armor >= 0)
		{
			player->inventory[player->equipped_armor].equipped = NO;
			player->equipped_armor = -1;
		}
	}

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
		PlaySound("sick");
		mvprintw(0,0,"Ugh! You ate a %s %s corpse! SICKENED! (+%d hunger) Polymorphed into %s!", 
			stage, corpse->name, nut, corpse->name);
	}
	else
	{
		PlaySound("polymorph");
		mvprintw(0,0,"Devoured %s %s! (+%d hunger) Polymorphed into %s! [%s]", 
			stage, corpse->name, nut, corpse->name, player->trait_desc);
	}

	int py = player->Position.yPos;
	int px = player->Position.xPos;
	if(GetCorpseAt(leveli[leveli->lvl], py, px) == corpse)
	{
		corpse->active = NO;
		if(leveli[leveli->lvl].map_backup[py][px] == '%')
		{
			leveli[leveli->lvl].map_backup[py][px] = '.';
		}
		player->Position.ch = leveli[leveli->lvl].map_backup[py][px];
	}
	else
	{
		corpse->active = NO;
	}

	PrintChar(py, px, leveli->player_symbol);
	move(py, px);

	PlayerInfoBar(leveli);
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

int GetPlayerAttack(Entity *player)
{
	int att = player->attack;
	if(player->can_open_doors == YES && player->equipped_weapon >= 0 && player->equipped_weapon < player->num_items)
	{
		att += player->inventory[player->equipped_weapon].bonus_att;
	}
	return att;
}

int GetPlayerDefense(Entity *player)
{
	int def = 0;
	if(player->can_open_doors == YES && player->equipped_armor >= 0 && player->equipped_armor < player->num_items)
	{
		def += player->inventory[player->equipped_armor].bonus_def;
	}
	return def;
}

int Combat(Level *leveli, Level level, Entity *player, Entity *monster, int attacker)
{
	char log_buf[256] = "";
	char tmp[128];

	if(attacker == PLAYER)
	{
		if(monster == NULL || monster->alive != YES)
		{
			return 0;
		}

		move(player->Position.yPos, player->Position.xPos);

		if(monster->health > 0)
		{
			// Check monster evasion (e.g. Bat, Ghost)
			if(monster->evasion_chance > 0 && (rand() % 100) < monster->evasion_chance)
			{
				PlaySound("miss");
				snprintf(tmp, sizeof(tmp), "The %s evades! ", monster->name);
				strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);
			}
			else
			{
				int dmg = GetPlayerAttack(player);
				int cleaved = 0;

				// Cleave trait
				if(player->cleave_chance > 0 && (rand() % 100) < player->cleave_chance)
				{
					dmg += 3;
					cleaved = 1;
				}

				monster->health -= dmg;
				PlaySound(cleaved ? "cleave" : "hit");
				if(cleaved)
				{
					snprintf(tmp, sizeof(tmp), "CLEAVE! Hit %s for %d dmg! ", monster->name, dmg);
				}
				else
				{
					snprintf(tmp, sizeof(tmp), "Hit %s for %d dmg! ", monster->name, dmg);
				}
				strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);

				// Bat / Lich Life Drain trait
				if(player->life_drain > 0 && player->health < player->maxhp)
				{
					int dr = player->life_drain;
					player->health += dr;
					if(player->health > player->maxhp) player->health = player->maxhp;
					snprintf(tmp, sizeof(tmp), "(+%d HP drain) ", dr);
					strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);
				}

				// Slime Acid Corrosion retaliation on hit
				if(monster->acid_corrosion > 0)
				{
					player->health -= 1;
					strncat(log_buf, "[Acid burn -1 HP] ", sizeof(log_buf) - strlen(log_buf) - 1);
					if(player->health <= 0)
					{
						player->alive = DEAD;
						leveli->killer = "Caustic Slime Acid";
					}
				}

				// Snake Venomous Strike trait
				if(player->poison_attack && monster->symbol != 's' && monster->symbol != 'B' && monster->symbol != 'L' && monster->health > 0)
				{
					monster->poison_turns = 3;
					strncat(log_buf, "(Venom!) ", sizeof(log_buf) - strlen(log_buf) - 1);
				}

				// Spider Skitter trait: second attack
				if(player->extra_attack_chance > 0 && (rand() % 100) < player->extra_attack_chance && monster->health > 0)
				{
					int second_dmg = GetPlayerAttack(player);
					monster->health -= second_dmg;
					snprintf(tmp, sizeof(tmp), "SKITTER 2nd hit +%d dmg! ", second_dmg);
					strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);
				}
			}

			if(monster->health <= 0)
			{
				int was_dragon = (monster->symbol == 'D');
				int was_miniboss = (monster->is_elite == 2 && !was_dragon);
				int old_xp = player->xp;
				monster->alive = DEAD;
				player->xp += monster->attack;
				int ranked_up = CheckXPUpgrade(player, old_xp);

				DropCorpse(leveli, monster, monster->Position.yPos, monster->Position.xPos);
				const char *dropped_loot = PopLastLootDrop();
				monster->Position.yPos = -1;
				monster->Position.xPos = -1;

				if(was_dragon)
				{
					PlaySound("boss_kill");
					snprintf(tmp, sizeof(tmp), "SLAIN %s (+%d XP)! A Golden Portal (*) appears!", monster->name, monster->attack);
				}
				else if(was_miniboss)
				{
					PlaySound("boss_kill");
					snprintf(tmp, sizeof(tmp), "VANQUISHED MINI-BOSS %s (+%d XP)!%s",
						monster->name, monster->attack, dropped_loot ? "" : "");
				}
				else if(ranked_up > 0)
				{
					snprintf(tmp, sizeof(tmp), "Killed %s (+%d XP)! [RANK UP! +3 HP, +1 ATK]", monster->name, monster->attack);
				}
				else
				{
					PlaySound("kill");
					snprintf(tmp, sizeof(tmp), "Killed %s (+%d XP)!", monster->name, monster->attack);
				}
				strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);
				if(dropped_loot != NULL)
				{
					snprintf(tmp, sizeof(tmp), " [LOOT DROP: %s!]", dropped_loot);
					strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);
				}

				ClearMsg();
				mvprintw(0, 0, "%s", log_buf);
				PlayerInfoBar(leveli);
				refresh();
				return 1;
			}

			// If the monster is not FOLLOW, it counter-attacks immediately here:
			if(monster->type != FOLLOW)
			{
				if(player->evasion_chance > 0 && (rand() % 100) < player->evasion_chance)
				{
					snprintf(tmp, sizeof(tmp), "| You evade %s! ", monster->name);
					strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);
				}
				else
				{
					int m_dmg = monster->attack;
					if(monster->cleave_chance > 0 && (rand() % 100) < monster->cleave_chance)
					{
						m_dmg += 3;
					}

					m_dmg -= GetPlayerDefense(player);
					if(m_dmg < 1) m_dmg = 1;

					player->health -= m_dmg;
					snprintf(tmp, sizeof(tmp), "| %s hits you for %d! ", monster->name, m_dmg);
					strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);

					if(player->acid_corrosion > 0)
					{
						monster->health -= 1;
						if(monster->health <= 0)
						{
							int old_xp = player->xp;
							monster->alive = DEAD;
							player->xp += monster->attack;
							CheckXPUpgrade(player, old_xp);
							DropCorpse(leveli, monster, monster->Position.yPos, monster->Position.xPos);
							monster->Position.yPos = -1;
							monster->Position.xPos = -1;
							snprintf(tmp, sizeof(tmp), "(Dissolved! +%d XP) ", monster->attack);
							strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);
						}
					}

					if(monster->alive == YES && monster->life_drain > 0 && monster->health < monster->maxhp)
					{
						monster->health += monster->life_drain;
						if(monster->health > monster->maxhp) monster->health = monster->maxhp;
					}

					if(monster->alive == YES && monster->poison_attack && player->symbol != 's' && player->symbol != 'B' && player->symbol != 'L' && player->health > 0)
					{
						player->poison_turns = 3;
						strncat(log_buf, "(Poisoned!) ", sizeof(log_buf) - strlen(log_buf) - 1);
					}
				}
			}

			if(monster->alive == YES)
			{
				char status_tag[64] = "";
				if(monster->poison_turns > 0 && monster->snare_turns > 0)
					snprintf(status_tag, sizeof(status_tag), " [PSN:%dt, WEB:%dt]", monster->poison_turns, monster->snare_turns);
				else if(monster->poison_turns > 0)
					snprintf(status_tag, sizeof(status_tag), " [PSN:%dt]", monster->poison_turns);
				else if(monster->snare_turns > 0)
					snprintf(status_tag, sizeof(status_tag), " [WEB:%dt]", monster->snare_turns);
				snprintf(tmp, sizeof(tmp), "[%s: %d/%d HP%s]", monster->name, monster->health, monster->maxhp, status_tag);
				strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);
			}
		}

		ClearMsg();
		mvprintw(0, 0, "%s", log_buf);
		PlayerInfoBar(leveli);
		refresh();
	}

	if(attacker == MONSTER)
	{
		if(monster == NULL || monster->alive != YES)
		{
			return 0;
		}

		if(monster->health > 0)
		{
			if(player->evasion_chance > 0 && (rand() % 100) < player->evasion_chance)
			{
				PlaySound("miss");
				snprintf(tmp, sizeof(tmp), "You evade the %s's attack! [%s: %d/%d HP]", monster->name, monster->name, monster->health, monster->maxhp);
				strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);
			}
			else
			{
				int m_dmg = monster->attack;
				int m_cleave = 0;
				int m_fire = 0;
				if(monster->symbol == 'D' && (rand() % 100) < 35)
				{
					m_dmg += 4;
					m_fire = 1;
				}
				else if(monster->cleave_chance > 0 && (rand() % 100) < monster->cleave_chance)
				{
					m_dmg += 3;
					m_cleave = 1;
				}

				m_dmg -= GetPlayerDefense(player);
				if(m_dmg < 1) m_dmg = 1;

				player->health -= m_dmg;
				PlaySound(m_fire ? "fire" : (m_cleave ? "cleave" : "hurt"));
				if(m_fire)
				{
					snprintf(tmp, sizeof(tmp), "%s breathes DRAGON FIRE for %d dmg! ", monster->name, m_dmg);
				}
				else if(m_cleave)
				{
					snprintf(tmp, sizeof(tmp), "%s CLEAVES you for %d dmg! ", monster->name, m_dmg);
				}
				else
				{
					snprintf(tmp, sizeof(tmp), "%s hits you for %d dmg! ", monster->name, m_dmg);
				}
				strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);

				// Player acid/fire reflection if Slime or Dragon
				if(player->acid_corrosion > 0)
				{
					monster->health -= 1;
					strncat(log_buf, "(Reflects 1 dmg) ", sizeof(log_buf) - strlen(log_buf) - 1);
					if(monster->health <= 0)
					{
						int old_xp = player->xp;
						monster->alive = DEAD;
						player->xp += monster->attack;
						CheckXPUpgrade(player, old_xp);
						DropCorpse(leveli, monster, monster->Position.yPos, monster->Position.xPos);
						monster->Position.yPos = -1;
						monster->Position.xPos = -1;
						snprintf(tmp, sizeof(tmp), "(Enemy dissolved! +%d XP) ", monster->attack);
						strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);
					}
				}

				// Bat / Lich monster life drain
				if(monster->alive == YES && monster->life_drain > 0 && monster->health < monster->maxhp)
				{
					monster->health += monster->life_drain;
					if(monster->health > monster->maxhp) monster->health = monster->maxhp;
				}

				if(monster->alive == YES && monster->poison_attack && player->symbol != 's' && player->symbol != 'B' && player->symbol != 'L' && player->health > 0)
				{
					player->poison_turns = 3;
					strncat(log_buf, "(Poisoned!) ", sizeof(log_buf) - strlen(log_buf) - 1);
				}

				if(monster->alive == YES && monster->extra_attack_chance > 0 && (rand() % 100) < monster->extra_attack_chance && player->health > 0)
				{
					int second_m = monster->attack - GetPlayerDefense(player);
					if(second_m < 1) second_m = 1;
					player->health -= second_m;
					snprintf(tmp, sizeof(tmp), "(2nd bite: %d dmg!) ", second_m);
					strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);
				}

				if(monster->alive == YES)
				{
					char status_tag[64] = "";
					if(monster->poison_turns > 0 && monster->snare_turns > 0)
						snprintf(status_tag, sizeof(status_tag), " [PSN:%dt, WEB:%dt]", monster->poison_turns, monster->snare_turns);
					else if(monster->poison_turns > 0)
						snprintf(status_tag, sizeof(status_tag), " [PSN:%dt]", monster->poison_turns);
					else if(monster->snare_turns > 0)
						snprintf(status_tag, sizeof(status_tag), " [WEB:%dt]", monster->snare_turns);
					snprintf(tmp, sizeof(tmp), "[%s: %d/%d HP%s]", monster->name, monster->health, monster->maxhp, status_tag);
					strncat(log_buf, tmp, sizeof(log_buf) - strlen(log_buf) - 1);
				}
			}

			ClearMsg();
			mvprintw(0, 0, "%s", log_buf);
			PlayerInfoBar(leveli);
			refresh();
		}
	}

	if(player->health <= 0)
	{
		player->alive = DEAD;
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
			int dy[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
			int dx[8] = {0, 0, -1, 1, 1, -1, 1, -1};
			int hits = 0;
			int webs_placed = 0;
			for(int i = 0; i < 8; i++)
			{
				int ny = py + dy[i];
				int nx = px + dx[i];
				Entity *mon = GetMonsterAt(ny, nx, level);
				if(mon != NULL && mon->alive == YES)
				{
					mon->snare_turns += 3;
					mon->poison_turns += 2;
					hits++;
				}
				else if(ny > 0 && ny < MAX_Y - 1 && nx > 0 && nx < MAX_X - 1 &&
				        leveli[leveli->lvl].map_backup != NULL &&
				        leveli[leveli->lvl].map_backup[ny][nx] == '.' &&
				        GetCorpseAt(leveli[leveli->lvl], ny, nx) == NULL &&
				        GetItemAt(leveli[leveli->lvl], ny, nx) == NULL &&
				        webs_placed < 4)
				{
					leveli[leveli->lvl].map_backup[ny][nx] = '^';
					PrintChar(ny, nx, '^');
					webs_placed++;
				}
			}
			if(hits > 0)
			{
				mvprintw(0,0,"You shoot sticky webbing! Ensnared %d foes (3t web, 2t poison) & spun %d web traps (^)!", hits, webs_placed);
			}
			else
			{
				mvprintw(0,0,"You spin %d sticky web traps (^) around you, setting a snare perimeter!", webs_placed);
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
				int dmg = player->attack + 2;
				target->health -= dmg;
				target->poison_turns += 4;
				if(target->health <= 0)
				{
					int old_xp = player->xp;
					target->alive = DEAD;
					player->xp += target->attack;
					CheckXPUpgrade(player, old_xp);
					DropCorpse(leveli, target, target->Position.yPos, target->Position.xPos);
					target->Position.yPos = -1;
					target->Position.xPos = -1;
					mvprintw(0,0,"You spit toxic venom at %s for %d dmg! Killed %s (+%d XP)!", target->name, dmg, target->name, target->attack);
				}
				else
				{
					mvprintw(0,0,"You spit toxic venom at %s! (%d damage, 4t poison) [%s: %d/%d HP]", target->name, dmg, target->name, target->health, target->maxhp);
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
			int kills = 0;
			int xp_gain = 0;
			for(int i = 0; i < 8; i++)
			{
				Entity *mon = GetMonsterAt(py + dy[i], px + dx[i], level);
				if(mon != NULL && mon->alive == YES)
				{
					mon->health -= 2;
					mon->snare_turns += 2;
					blinded++;
					if(mon->health <= 0)
					{
						int old_xp = player->xp;
						mon->alive = DEAD;
						player->xp += mon->attack;
						CheckXPUpgrade(player, old_xp);
						xp_gain += mon->attack;
						kills++;
						DropCorpse(leveli, mon, mon->Position.yPos, mon->Position.xPos);
						mon->Position.yPos = -1;
						mon->Position.xPos = -1;
					}
				}
			}
			if(kills > 0)
			{
				mvprintw(0,0,"BANG! Flashbang hits %d foes! (%d killed, +%d XP!)", blinded, kills, xp_gain);
			}
			else
			{
				mvprintw(0,0,"BANG! Tinkered flashbang detonates! Blinded %d nearby foes!", blinded);
			}
			used = 1;
			break;
		}

		case 'T': // Troll: Ground Slam
		{
			ClearMsg();
			int dy[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
			int dx[8] = {0, 0, -1, 1, 1, -1, 1, -1};
			int hit_count = 0;
			int kills = 0;
			int xp_gain = 0;
			for(int i = 0; i < 8; i++)
			{
				Entity *mon = GetMonsterAt(py + dy[i], px + dx[i], level);
				if(mon != NULL && mon->alive == YES)
				{
					mon->health -= 7;
					hit_count++;
					if(mon->health <= 0)
					{
						int old_xp = player->xp;
						mon->alive = DEAD;
						player->xp += mon->attack;
						CheckXPUpgrade(player, old_xp);
						xp_gain += mon->attack;
						kills++;
						DropCorpse(leveli, mon, mon->Position.yPos, mon->Position.xPos);
						mon->Position.yPos = -1;
						mon->Position.xPos = -1;
					}
				}
			}
			if(kills > 0)
			{
				mvprintw(0,0,"ROAR! GROUND SLAM crushes %d foes for 7 dmg! (%d killed, +%d XP!)", hit_count, kills, xp_gain);
			}
			else
			{
				mvprintw(0,0,"ROAR! GROUND SLAM! Tremor crushes %d surrounding enemies for 7 damage!", hit_count);
			}
			used = 1;
			break;
		}

		case 'G': // Ghost: Shadow Blink
		{
			ClearMsg();
			int start_dir = rand() % 4;
			int dy[4] = {-3, 3, 0, 0};
			int dx[4] = {0, 0, -3, 3};
			int blinked = 0;
			for(int attempt = 0; attempt < 4; attempt++)
			{
				int dir = (start_dir + attempt) % 4;
				int ny = py + dy[dir];
				int nx = px + dx[dir];
				if(ny > 0 && ny < MAX_Y - 1 && nx > 0 && nx < MAX_X - 1 &&
				   GetMonsterAt(ny, nx, level) == NULL)
				{
					PrintChar(py, px, level.map_backup[py][px]);
					player->Position.yPos = ny;
					player->Position.xPos = nx;
					player->Position.ch = level.map_backup[ny][nx];
					PrintChar(ny, nx, leveli->player_symbol);
					UpdateFOV(&leveli[leveli->lvl]);
					RenderFOV(&leveli[leveli->lvl]);
					move(ny, nx);
					mvprintw(0,0,"You ethereal-warp across space through the shadows!");
					blinked = 1;
					used = 1;
					break;
				}
			}
			if(!blinked)
			{
				mvprintw(0,0,"Shadow Blink blocked in all directions! (No hunger or cooldown spent)");
				used = 0;
			}
			break;
		}

		case 'B': // Slime: Acid Jet
		{
			ClearMsg();
			int dy[4] = {-1, 1, 0, 0};
			int dx[4] = {0, 0, -1, 1};
			int dissolved = 0;
			int kills = 0;
			int xp_gain = 0;
			int pools = 0;
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
								int old_xp = player->xp;
								mon->alive = DEAD;
								player->xp += mon->attack;
								CheckXPUpgrade(player, old_xp);
								xp_gain += mon->attack;
								kills++;
								DropCorpse(leveli, mon, mon->Position.yPos, mon->Position.xPos);
								mon->Position.yPos = -1;
								mon->Position.xPos = -1;
							}
						}
						else if(step == 1 && ny < MAX_Y - 1 && nx < MAX_X - 1 &&
						        leveli[leveli->lvl].map_backup != NULL &&
						        leveli[leveli->lvl].map_backup[ny][nx] == '.' &&
						        GetCorpseAt(leveli[leveli->lvl], ny, nx) == NULL &&
						        GetItemAt(leveli[leveli->lvl], ny, nx) == NULL)
						{
							leveli[leveli->lvl].map_backup[ny][nx] = '=';
							PrintChar(ny, nx, '=');
							pools++;
						}
					}
				}
			}
			if(kills > 0)
			{
				mvprintw(0,0,"SPLAT! Acid Jet dissolves %d targets (%d killed, +%d XP) & leaves %d acid pools (=)!", dissolved, kills, xp_gain, pools);
			}
			else
			{
				mvprintw(0,0,"SPLAT! Acid Jet hits %d targets for 5 dmg & leaves %d caustic acid pools (=)!", dissolved, pools);
			}
			used = 1;
			break;
		}

		case 'L': // Necromancer Lich: Soul Harvest
		{
			ClearMsg();
			int drained = 0;
			int kills = 0;
			int xp_gain = 0;
			int healed = 0;
			for(int m = 0; m < level.mons; m++)
			{
				Entity *mon = level.monsters[m];
				if(mon != NULL && mon->alive == YES)
				{
					int dist = abs(mon->Position.yPos - py) + abs(mon->Position.xPos - px);
					if(dist <= 3)
					{
						mon->health -= 5;
						drained++;
						int prev_hp = player->health;
						player->health += 3;
						if(player->health > player->maxhp) player->health = player->maxhp;
						healed += (player->health - prev_hp);
						if(mon->health <= 0)
						{
							int old_xp = player->xp;
							mon->alive = DEAD;
							player->xp += mon->attack;
							CheckXPUpgrade(player, old_xp);
							xp_gain += mon->attack;
							kills++;
							DropCorpse(leveli, mon, mon->Position.yPos, mon->Position.xPos);
							mon->Position.yPos = -1;
							mon->Position.xPos = -1;
						}
					}
				}
			}
			if(kills > 0)
			{
				mvprintw(0,0,"SOUL HARVEST! Siphoned %d foes for 5 dmg (+%d HP healed, %d slain, +%d XP)!", drained, healed, kills, xp_gain);
			}
			else
			{
				mvprintw(0,0,"SOUL HARVEST! Necrotic wave drains %d nearby foes for 5 dmg (+%d HP healed)!", drained, healed);
			}
			used = 1;
			break;
		}

		case 'D': // Ancient Dragon: Dragon Breath
		{
			ClearMsg();
			int scorched = 0;
			int kills = 0;
			int xp_gain = 0;
			for(int m = 0; m < level.mons; m++)
			{
				Entity *mon = level.monsters[m];
				if(mon != NULL && mon->alive == YES)
				{
					int dist = abs(mon->Position.yPos - py) + abs(mon->Position.xPos - px);
					if(dist <= 3)
					{
						mon->health -= 12;
						scorched++;
						if(mon->health <= 0)
						{
							int old_xp = player->xp;
							mon->alive = DEAD;
							player->xp += mon->attack;
							CheckXPUpgrade(player, old_xp);
							xp_gain += mon->attack;
							kills++;
							DropCorpse(leveli, mon, mon->Position.yPos, mon->Position.xPos);
							mon->Position.yPos = -1;
							mon->Position.xPos = -1;
						}
					}
				}
			}
			if(kills > 0)
			{
				mvprintw(0,0,"INFERNO! Dragon Breath incinerates %d foes for 12 dmg! (%d killed, +%d XP!)", scorched, kills, xp_gain);
			}
			else
			{
				mvprintw(0,0,"INFERNO! You unleash Dragon Breath, scorching %d nearby foes for 12 dmg!", scorched);
			}
			used = 1;
			break;
		}

		case 'V': // Vampire Bat: Echo Screech
		{
			ClearMsg();
			int detected = 0;
			for(int m = 0; m < leveli[leveli->lvl].mons; m++)
			{
				Entity *mon = leveli[leveli->lvl].monsters[m];
				if(mon != NULL && mon->alive == YES)
				{
					int my = mon->Position.yPos;
					int mx = mon->Position.xPos;
					detected++;
					for(int ry = my - 1; ry <= my + 1; ry++)
					{
						for(int rx = mx - 1; rx <= mx + 1; rx++)
						{
							if(ry >= 0 && ry < MAX_Y && rx >= 0 && rx < MAX_X)
							{
								if(leveli[leveli->lvl].discovered != NULL)
									leveli[leveli->lvl].discovered[ry][rx] = 1;
								if(leveli[leveli->lvl].visible != NULL)
									leveli[leveli->lvl].visible[ry][rx] = 1;
							}
						}
					}
				}
			}
			RenderFOV(&leveli[leveli->lvl]);
			mvprintw(0,0,"SKREEE! Echolocation pulse reveals %d creatures across this floor!", detected);
			used = 1;
			break;
		}

		default:
			break;
	}

	if(used)
	{
		PlaySound(player->symbol == 'D' ? "fire" : "ability");
		player->hunger -= player->ability_cost;
		if(player->hunger < 0) player->hunger = 0;
		player->ability_cd = player->ability_max_cd;
		PlayerInfoBar(leveli);
		refresh();
		if(player->symbol == 'V')
		{
			getch(); // Let the player inspect the revealed monster locations before monsters move
		}
		return 1;
	}

	return 0;
}

int AreMonstersRivals(Entity *m1, Entity *m2)
{
	if(m1 == NULL || m2 == NULL || m1 == m2) return 0;
	if(m1->alive != YES || m2->alive != YES) return 0;
	if(m1->symbol == 'D' || m2->symbol == 'D')
	{
		return (m1->frenzy_turns > 0 || m2->frenzy_turns > 0) ? 1 : 0;
	}
	if(m1->frenzy_turns > 0 || m2->frenzy_turns > 0) return 1;
	if(m1->symbol == m2->symbol) return 0;

	char s1 = m1->symbol;
	char s2 = m2->symbol;
	// Natural Dungeon Rivalries:
	// 1. Spiders ('x') vs Snakes ('s') & Vampire Bats ('V')
	if((s1 == 'x' && (s2 == 's' || s2 == 'V')) || (s2 == 'x' && (s1 == 's' || s1 == 'V'))) return 1;
	// 2. Goblins ('g') vs Acid Slimes ('B'), Spiders ('x'), and Ghosts ('G')
	if((s1 == 'g' && (s2 == 'B' || s2 == 'x' || s2 == 'G')) || (s2 == 'g' && (s1 == 'B' || s1 == 'x' || s1 == 'G'))) return 1;
	// 3. Trolls ('T') vs Goblins ('g') and Necromancer Liches ('L')
	if((s1 == 'T' && (s2 == 'g' || s2 == 'L')) || (s2 == 'T' && (s1 == 'g' || s1 == 'L'))) return 1;
	// 4. Acid Slimes ('B') dissolve Snakes ('s') and Trolls ('T')
	if((s1 == 'B' && (s2 == 's' || s2 == 'T')) || (s2 == 'B' && (s1 == 's' || s1 == 'T'))) return 1;
	return 0;
}

int MonsterInfight(Level *leveli, Level level, Entity *attacker, Entity *defender)
{
	if(leveli == NULL || attacker == NULL || defender == NULL) return 0;
	if(attacker->alive != YES || defender->alive != YES) return 0;

	Level *cur = &leveli[leveli->lvl];
	int ay = attacker->Position.yPos;
	int ax = attacker->Position.xPos;
	int dy = defender->Position.yPos;
	int dx = defender->Position.xPos;
	int is_vis = 0;
	if(cur->visible != NULL)
	{
		if((ay >= 0 && ay < MAX_Y && ax >= 0 && ax < MAX_X && cur->visible[ay][ax]) ||
		   (dy >= 0 && dy < MAX_Y && dx >= 0 && dx < MAX_X && cur->visible[dy][dx]))
		{
			is_vis = 1;
		}
	}

	int dmg = attacker->attack;
	if(attacker->cleave_chance > 0 && (rand() % 100) < attacker->cleave_chance)
	{
		dmg += 3;
	}
	defender->health -= dmg;

	if(attacker->life_drain > 0 && attacker->health < attacker->maxhp)
	{
		attacker->health += attacker->life_drain;
		if(attacker->health > attacker->maxhp) attacker->health = attacker->maxhp;
	}
	if(attacker->poison_attack && defender->symbol != 's' && defender->symbol != 'B' && defender->symbol != 'L' && defender->health > 0)
	{
		defender->poison_turns += 2;
	}

	if(defender->health <= 0)
	{
		defender->alive = DEAD;
		int xp_bonus = (defender->attack + 1) / 2;
		int old_xp = cur->player->xp;
		cur->player->xp += xp_bonus;
		CheckXPUpgrade(cur->player, old_xp);

		DropCorpse(leveli, defender, dy, dx);
		const char *dropped_loot = PopLastLootDrop();
		defender->Position.yPos = -1;
		defender->Position.xPos = -1;

		if(is_vis)
		{
			PlaySound("kill");
			ClearMsg();
			if(dropped_loot != NULL)
			{
				mvprintw(0, 0, "RIVALRY! %s slays %s in combat! (+%d XP, dropped %s!)",
					attacker->name, defender->name, xp_bonus, dropped_loot);
			}
			else
			{
				mvprintw(0, 0, "RIVALRY! %s slays %s in a dungeon brawl! (+%d XP, corpse dropped!)",
					attacker->name, defender->name, xp_bonus);
			}
			PlayerInfoBar(leveli);
			refresh();
		}
	}
	else if(is_vis)
	{
		PlaySound("hit");
		ClearMsg();
		mvprintw(0, 0, "CLASH! %s attacks rival %s for %d dmg! [%s: %d/%d HP]",
			attacker->name, defender->name, dmg, defender->name, defender->health, defender->maxhp);
		refresh();
	}
	return 1;
}
 
