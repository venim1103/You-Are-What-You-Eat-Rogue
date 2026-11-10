#include "global.h"

int HandleInput(Level *leveli, Level level, int *yx)
{
	int orig_y = level.player->Position.yPos;
	int orig_x = level.player->Position.xPos;
	int target_y = orig_y;
	int target_x = orig_x;
	int is_move = 0;
	int action_taken = 0;

	switch(level.player->input)
	{
		// Cardinal & diagonal movement (WASD, vi keys, and numpad):
		case 'w': case 'k': case '8':
			target_y = orig_y - 1; target_x = orig_x; is_move = 1; break;
		case 's': case 'j': case '2':
			target_y = orig_y + 1; target_x = orig_x; is_move = 1; break;
		case 'a': case 'h': case '4':
			target_y = orig_y; target_x = orig_x - 1; is_move = 1; break;
		case 'd': case 'l': case '6':
			target_y = orig_y; target_x = orig_x + 1; is_move = 1; break;
		case 'q': case 'y': case '7':
			target_y = orig_y - 1; target_x = orig_x - 1; is_move = 1; break;
		case 'z': case 'b': case '1':
			target_y = orig_y + 1; target_x = orig_x - 1; is_move = 1; break;
		case 'x': case 'n': case '3':
			target_y = orig_y + 1; target_x = orig_x + 1; is_move = 1; break;
		case 'u': case '9':
			target_y = orig_y - 1; target_x = orig_x + 1; is_move = 1; break;

		// 'e': if standing on corpse, eat it; otherwise move upright (standard WASD+QEZX)
		case 'e':
			if(GetCorpseAt(level, orig_y, orig_x) != NULL)
			{
				action_taken = AskEatCorpse(leveli, level, level.player, orig_y, orig_x);
			}
			else
			{
				target_y = orig_y - 1; target_x = orig_x + 1; is_move = 1;
			}
			break;

		// 'E': eat command (floor corpse or backpack corpse)
		case 'E':
			if(GetCorpseAt(level, orig_y, orig_x) != NULL)
			{
				action_taken = AskEatCorpse(leveli, level, level.player, orig_y, orig_x);
			}
			else
			{
				action_taken = EatFromInventory(leveli, level, level.player);
			}
			break;

		// Inventory Menu ('i' or 'I'):
		case 'i':
		case 'I':
			ShowInventory(leveli, level, level.player);
			action_taken = 0; // Menu view doesn't waste a turn
			break;

		// Bestiary & Form Compendium ('B'):
		case 'B':
			ShowBestiary(leveli, level, level.player);
			action_taken = 0; // Compendium view doesn't waste a turn
			break;

		// Look / Examine Mode ('v', 'V', ';', '/'):
		case 'v':
		case 'V':
		case ';':
		case '/':
			LookMode(leveli, level, level.player);
			action_taken = 0; // Look mode doesn't waste a turn
			break;

		// Quick Wield ('W'):
		case 'W':
			action_taken = QuickWield(leveli, level, level.player);
			break;

		// Quick Pickup ('g' or ','):
		case 'g':
		case ',':
			action_taken = PickUpFloorItem(leveli, level, level.player);
			break;

		// Quick Drop ('D'):
		case 'D':
			action_taken = QuickDrop(leveli, level, level.player);
			break;

		// Form Active Ability ('f' or 'a'):
		case 'f':
		case 'F':
			action_taken = ActivateFormAbility(leveli, level, level.player);
			break;

		// Use Stairs / Golden Portal ('<', '>', 'r', 'R'):
		case '<':
		case '>':
		case 'r':
		case 'R':
		{
			char cur_tile = (level.map_backup != NULL) ? level.map_backup[orig_y][orig_x] : '.';
			if(cur_tile == '<')
			{
				AskStairs(DSTAIRS, level.player);
			}
			else if(cur_tile == '>')
			{
				AskStairs(USTAIRS, level.player);
			}
			else if(cur_tile == '*')
			{
				ClearMsg();
				mvprintw(0, 0, "Step through the Golden Portal (*) and claim VICTORY? (y/n)");
				refresh();
				int ans = getch();
				ClearMsg();
				if(ans == 'y' || ans == 'Y')
				{
					level.player->Position.type = VICTORY;
				}
			}
			else if(cur_tile == '_' || cur_tile == '~')
			{
				action_taken = UseShrine(leveli, level, level.player, orig_y, orig_x);
			}
			else
			{
				ClearMsg();
				mvprintw(0, 0, "There are no stairs or shrines here. (Stand on '<', '>', '_', or '~')");
				refresh();
			}
			action_taken = 0;
			break;
		}

		// Rest / Wait a turn:
		case '.':
		case ' ':
		case 10:
		case 13:
		case '5':
			action_taken = 1;
			break;

		// Close door (player can always operate doors regardless of form):
		case 'c': 
			TurnNearby(orig_y, orig_x, ':', "+", level);
			ClearMsg();
			mvprintw(0,0,"You close the nearby doors.");
			refresh();
			action_taken = 1;
			break;

		case '?':
			LookMode(leveli, level, level.player);
			action_taken = 0;
			break;

		default:
			break;
	}

	if(is_move)
	{
		// Clear previous turn's message when moving:
		ClearMsg();

		if(target_y < 1 || target_y >= MAX_Y || target_x < 0 || target_x >= MAX_X)
		{
			mvprintw(0, 0, "You bump into the dungeon boundary.");
			refresh();
			action_taken = 0;
		}
		else
		{
		// 1. Check if an active monster is at target tile:
		Entity *target = GetMonsterAt(target_y, target_x, level);
		if(target != NULL && target->alive == YES)
		{
			Combat(leveli, level, level.player, target, PLAYER);
			action_taken = 1;
		}
		else
		{
			chtype ch = ' ';
			if(level.map_backup != NULL && target_y >= 0 && target_y < MAX_Y && target_x >= 0 && target_x < MAX_X)
			{
				ch = level.map_backup[target_y][target_x];
			}

			switch(ch)
			{
				case '.':
				case '#':
				case ':':
				case '>':
				case '<':
				case '*':
				case '_':
				case '~':
				case '^':
				case '=':
				case '%':
				case ')':
				case '[':
				case '!':
				case '?':
				{
					if(ch == '+')
					{
						action_taken = OpenDoor(leveli, level.player, target_y, target_x);
						break;
					}
					StepOn(leveli, PLAYER, target_y, target_x, ch, leveli->player_symbol, level, NULL);
					level.player->Position.type = (ch == ':') ? DOOR : GROUND;
					action_taken = 1;

					// Check dynamic entities at target tile:
					Corpse *corp = GetCorpseAt(level, target_y, target_x);
					Item *it = GetItemAt(level, target_y, target_x);
					if(ch == '*')
					{
						ClearMsg();
						mvprintw(0, 0, "Step through the Golden Portal (*) and claim VICTORY? (y/n)");
						refresh();
						int ans = getch();
						ClearMsg();
						if(ans == 'y' || ans == 'Y')
						{
							level.player->Position.type = VICTORY;
						}
						else
						{
							mvprintw(0, 0, "You stand beside the shimmering Golden Portal (*). Press 'r' or '>' to escape.");
							refresh();
						}
					}
					else if(ch == '>')
					{
						AskStairs(USTAIRS, level.player);
					}
					else if(ch == '<')
					{
						AskStairs(DSTAIRS, level.player);
					}
					else if(ch == '_' || ch == '~')
					{
						UseShrine(leveli, level, level.player, target_y, target_x);
					}
					else if(ch == '^' || ch == '=')
					{
						TriggerFloorHazard(leveli, level, level.player, 1, target_y, target_x);
					}
					else if(corp != NULL && corp->active == YES)
					{
						level.player->Position.type = LOOT;
						AskEatCorpse(leveli, level, level.player, target_y, target_x);
					}
					else if(it != NULL && it->active == YES)
					{
						ClearMsg();
						if(it->item_type == ITEM_WEAPON)
						{
							mvprintw(0, 0, "You see %s (+%d ATK) here. Press 'g' to pick up.", it->name, it->bonus_att);
						}
						else if(it->item_type == ITEM_ARMOR)
						{
							mvprintw(0, 0, "You see %s (+%d DEF) here. Press 'g' to pick up.", it->name, it->bonus_def);
						}
						else if(it->item_type == ITEM_CONSUMABLE)
						{
							mvprintw(0, 0, "You see %s (%c) [%s]. Press 'g' to pick up.", it->name, it->symbol, GetConsumableDesc(it->bonus_att));
						}
						refresh();
					}
					break;
				}
				case '+':
					action_taken = OpenDoor(leveli, level.player, target_y, target_x);
					break;
				default:
					if(level.player->can_phase_walls && (target_y > 0 && target_y < MAX_Y && target_x > 0 && target_x < MAX_X))
					{
						StepOn(leveli, PLAYER, target_y, target_x, ch, leveli->player_symbol, level, NULL);
						ClearMsg();
						mvprintw(0, 0, "You phase ethereal through the wall!");
						refresh();
						action_taken = 1;
					}
					else
					{
						ClearMsg();
						mvprintw(0, 0, "You bump into a wall.");
						refresh();
						action_taken = 0;
					}
					break;
			}
		}
		}
	}

	// If a turn was taken, monsters take their turn:
	if(action_taken)
	{
		for(int i=0; i<level.mons; i++)
		{
			if(level.monsters[i] == NULL || level.monsters[i]->alive != YES)
			{
				continue;
			}
			int my = level.monsters[i]->Position.yPos;
			int mx = level.monsters[i]->Position.xPos;
			if(my < 1 || my >= MAX_Y - 1 || mx < 1 || mx >= MAX_X - 1)
			{
				continue;
			}
			int py = level.player->Position.yPos;
			int px = level.player->Position.xPos;
			int dist = abs(my - py) + abs(mx - px);
			if(level.monsters[i]->frenzy_turns > 0)
			{
				level.monsters[i]->frenzy_turns--;
			}

			// Ensnared/stunned monsters cannot move, though they can still melee if adjacent:
			if(level.monsters[i]->snare_turns > 0)
			{
				level.monsters[i]->snare_turns--;
				if(abs(my - py) <= 1 && abs(mx - px) <= 1)
				{
					Combat(leveli, level, level.player, level.monsters[i], MONSTER);
				}
				continue;
			}

			// Monster Infighting & Faction Rivalries: check for an adjacent rival monster
			Entity *rival = NULL;
			int best_rdist = 999;
			for(int j = 0; j < level.mons; j++)
			{
				if(i == j || level.monsters[j] == NULL || level.monsters[j]->alive != YES) continue;
				if(!AreMonstersRivals(level.monsters[i], level.monsters[j])) continue;
				int ry = level.monsters[j]->Position.yPos;
				int rx = level.monsters[j]->Position.xPos;
				int rdist = abs(my - ry) + abs(mx - rx);
				if(abs(my - ry) <= 1 && abs(mx - rx) <= 1)
				{
					rival = level.monsters[j];
					best_rdist = 1;
					break;
				}
				if((level.monsters[i]->frenzy_turns > 0 || dist > 3) && rdist <= 4 && rdist < best_rdist)
				{
					best_rdist = rdist;
					rival = level.monsters[j];
				}
			}
			if(rival != NULL && best_rdist == 1 && (level.monsters[i]->frenzy_turns > 0 || dist > 1))
			{
				MonsterInfight(leveli, level, level.monsters[i], rival);
				continue;
			}

			int is_vis = (level.visible != NULL && my >= 0 && my < MAX_Y && mx >= 0 && mx < MAX_X) ? level.visible[my][mx] : 0;

			if(rival != NULL && (level.monsters[i]->frenzy_turns > 0 || dist > 3))
			{
				yx = PathFindFollow(yx, my, mx, rival->Position.yPos, rival->Position.xPos, level.map_backup, level.monsters[i]->can_phase_walls);
			}
			else if(level.monsters[i]->type==FOLLOW && (dist <= 14 || is_vis))
			{
				yx = PathFindFollow(yx, my, mx, py, px, level.map_backup, level.monsters[i]->can_phase_walls);
			} else if(level.monsters[i]->mode==RANDOM_S)
			{
				yx = PathFindRandom(yx, my, mx, level.map_backup, RANDOM_S);
			} else {
				yx = PathFindRandom(yx, my, mx, level.map_backup, RANDOM_X);
			}
			
			my = yx[0]; mx = yx[1];
			level.monsters[i] = CheckNextChar(leveli, level.monsters[i]->symbol, MONSTER, my, mx, level, level.monsters[i]);
		}
	}

	move(level.player->Position.yPos, level.player->Position.xPos);
	refresh();
	return action_taken;
}




