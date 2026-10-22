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

		// Rest / Wait a turn:
		case '.':
		case ' ':
		case 10:
		case 13:
		case '5':
			action_taken = 1;
			break;

		// Close door:
		case 'c': 
			if(level.player->can_open_doors == NO)
			{
				ClearMsg();
				mvprintw(0,0,"You cannot operate doors as a %s! (You lack hands)", level.player->name);
				refresh();
			}
			else
			{
				TurnNearby(orig_y, orig_x, ':', "+", level);
				ClearMsg();
				mvprintw(0,0,"You close the nearby doors.");
				refresh();
				action_taken = 1;
			}
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

		// 1. Check if an active monster is at target tile:
		Entity *target = GetMonsterAt(target_y, target_x, level);
		if(target != NULL && target->alive == YES)
		{
			Combat(leveli, level, level.player, target, PLAYER);
			action_taken = 1;
		}
		else
		{
			chtype ch = '.';
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
				case '%':
				case ')':
				case '[':
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
					if(ch == '>')
					{
						AskStairs(USTAIRS, level.player);
					}
					else if(ch == '<')
					{
						AskStairs(DSTAIRS, level.player);
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

	// If a turn was taken, monsters take their turn:
	if(action_taken)
	{
		for(int i=0; i<level.mons; i++)
		{
			if(level.monsters[i]->alive != YES)
			{
				continue;
			}
			int my = level.monsters[i]->Position.yPos;
			int mx = level.monsters[i]->Position.xPos;
			int py = level.player->Position.yPos;
			int px = level.player->Position.xPos;
			int dist = abs(my - py) + abs(mx - px);
			int is_vis = (level.visible != NULL && my >= 0 && my < MAX_Y && mx >= 0 && mx < MAX_X) ? level.visible[my][mx] : 0;

			if(level.monsters[i]->type==FOLLOW && (dist <= 14 || is_vis))
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




