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

		// 'E': explicit eat command
		case 'E':
			if(GetCorpseAt(level, orig_y, orig_x) != NULL)
			{
				action_taken = AskEatCorpse(leveli, level, level.player, orig_y, orig_x);
			}
			else
			{
				ClearMsg();
				mvprintw(0,0,"There is no corpse here to eat.");
				refresh();
			}
			break;

		// Rest / Wait a turn:
		case '.':
		case ' ':
		case '5':
			action_taken = 1;
			break;

		// Close door:
		case 'c': 
			if(level.player->can_open_doors == NO)
			{
				ClearMsg();
				mvprintw(0,0,"You cannot operate doors as a %s! (You lack hands)", level.player->name);
				refresh(); getch();
				ClearMsg();
				refresh();
			}
			else
			{
				TurnNearby(orig_y, orig_x, ':', "+", level);
				ClearMsg();
				mvprintw(0,0,"You are closing nearby doors!");
				refresh();
				action_taken = 1;
			}
			break;

		case '?':
			getch();
			PrintMap(MAX_Y,MAX_X,level.map);
			refresh();
			getch();	
			break;

		default:
			break;
	}

	if(is_move)
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
			chtype ch = mvinch(target_y, target_x) & A_CHARTEXT;
			switch(ch)
			{
				case '.':
				case '#':
					StepOn(leveli, PLAYER, target_y, target_x, ch, leveli->player_symbol, level, NULL);
					level.player->Position.type = GROUND;
					action_taken = 1;
					break;
				case ':':
					StepOn(leveli, PLAYER, target_y, target_x, ch, leveli->player_symbol, level, NULL);
					level.player->Position.type = DOOR;
					action_taken = 1;
					break;
				case '+':
					action_taken = OpenDoor(level.player, target_y, target_x);
					break;
				case '>':
					StepOn(leveli, PLAYER, target_y, target_x, ch, leveli->player_symbol, level, NULL);
					AskStairs(USTAIRS, level.player);
					action_taken = 1;
					break;
				case '<':
					StepOn(leveli, PLAYER, target_y, target_x, ch, leveli->player_symbol, level, NULL);
					AskStairs(DSTAIRS, level.player);
					action_taken = 1;
					break;
				case '%':
					StepOn(leveli, PLAYER, target_y, target_x, ch, leveli->player_symbol, level, NULL);
					level.player->Position.type = LOOT;
					AskEatCorpse(leveli, level, level.player, target_y, target_x);
					action_taken = 1;
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
		char chk[10] = {'.','#',':','@','s', 'g','x','T'};
		for(int i=0; i<level.mons; i++)
		{
			if(level.monsters[i]->alive != YES)
			{
				continue;
			}
			int my = level.monsters[i]->Position.yPos;
			int mx = level.monsters[i]->Position.xPos;

			if(level.monsters[i]->type==FOLLOW)
			{
				yx = PathFindFollow(yx, my, mx, level.player->Position.yPos, level.player->Position.xPos, chk);
			} else if(level.monsters[i]->mode==RANDOM_S)
			{
				yx = PathFindRandom(yx, my, mx, chk, RANDOM_S);
			} else {
				yx = PathFindRandom(yx, my, mx, chk, RANDOM_X);
			}
			
			my = yx[0]; mx = yx[1];
			level.monsters[i] = CheckNextChar(leveli, level.monsters[i]->symbol, MONSTER, my, mx, level, level.monsters[i]);
		}
	}

	move(level.player->Position.yPos, level.player->Position.xPos);
	refresh();
	return action_taken;
}




