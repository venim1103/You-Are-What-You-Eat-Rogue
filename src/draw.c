#include "global.h"

Position DrawDoor(int height, int width, Position pos, int i)
{
// random number from 0to(width-1) thus requires to add +1 at the end to get between 1to(width-1), as no need for 2 corners the width has to become width-2 -> (originally width=width-1 so width-1=width-2)!
	int ranw = (rand() % (width-1))+1; 
	int ranh = (rand() % (height-1))+1; 

	Position door; 

	// Print door
	switch(i) 
	{
		case 0: // Top side
			door.yPos = pos.yPos; door.xPos = (pos.xPos+ranw);
			mvprintw(door.yPos, door.xPos, "+");
			break;
		case 1: // Bottom side
			door.yPos = (pos.yPos+height); door.xPos = (pos.xPos+ranw);
			mvprintw(door.yPos, door.xPos, "+");
			break;
		case 2: // Left side
			door.yPos = (pos.yPos+ranh); door.xPos = pos.xPos;
			mvprintw(door.yPos, door.xPos, "+");
			break;
		case 3: // Right side
			door.yPos = (pos.yPos+ranh); door.xPos = (pos.xPos+width);
			mvprintw(door.yPos, door.xPos, "+");
			break;
		default:
			break;
	}
	return door;	
}

Position *DrawRoom(Position pos, int d)
{	// Width and height are one too long! (BUT DON'T MINUS!!)
	int height = pos.height;
	int width = pos.width;

// Print walls and floor:
	// Top and bottom with - (1 more wide) 
	for(int i = 0; i < width+1; i++)
	{	
		mvprintw(pos.yPos ,pos.xPos+i, "-");
		mvprintw(pos.yPos+height, pos.xPos+i, "-");
	}	
	// Print the rest (because not top or bottom = ypos+1->height
	for(int i = 1; i < height; i++)
	{	
		mvprintw(pos.yPos+i, pos.xPos, "|");
	
		mvprintw(pos.yPos+i, pos.xPos+width, "|");
	
		for(int j = 1; j < width; j++)
		{	
			mvprintw(pos.yPos+i, pos.xPos+j, ".");
		}
	}
// Print doors:

	Position *doors;
	doors = malloc(sizeof(Position)*d); 

	for(int i=0;i<d;i++)
	{
		doors[i] = DrawDoor(height,width,pos,i);
	}

	return doors;	
}

int TurnNearby(int y, int x, int c, char *d, Level level)
{
	int dy[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
	int dx[8] = {0, 0, -1, 1, 1, -1, 1, -1};
	for(int i = 0; i < 8; i++)
	{
		int ny = y + dy[i];
		int nx = x + dx[i];
		if((mvinch(ny, nx) & A_CHARTEXT) == c)
		{
			mvprintw(ny, nx, "%s", d);
			if(level.map_backup != NULL)
			{
				level.map_backup[ny][nx] = d[0];
			}
		}
	}
	return 1;
}

static void CastRay(Level *level, int y0, int x0, int y1, int x1, int max_dist)
{
	int dy = abs(y1 - y0);
	int dx = abs(x1 - x0);
	int sy = (y0 < y1) ? 1 : -1;
	int sx = (x0 < x1) ? 1 : -1;
	int err = dx - dy;
	int cy = y0;
	int cx = x0;

	while(1)
	{
		int dist = abs(cy - y0) + abs(cx - x0);
		if (dist > max_dist) break;

		if (cy >= 0 && cy < MAX_Y && cx >= 0 && cx < MAX_X)
		{
			level->visible[cy][cx] = 1;
			level->discovered[cy][cx] = 1;

			char tile = (level->map_backup != NULL) ? level->map_backup[cy][cx] : ' ';
			if (tile == '-' || tile == '|' || tile == '+')
			{
				if (level->player == NULL || level->player->can_phase_walls == 0)
				{
					break;
				}
			}
		}

		if (cy == y1 && cx == x1) break;

		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			cx += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			cy += sy;
		}
	}
}

void UpdateFOV(Level *level)
{
	if(level == NULL || level->player == NULL || level->visible == NULL || level->discovered == NULL)
	{
		return;
	}

	int py = level->player->Position.yPos;
	int px = level->player->Position.xPos;
	int radius = 8;
	if (level->player->symbol == 'V')
	{
		radius = 12; // Vampire Bat echolocation
	}

	for(int r = 0; r < MAX_Y; r++)
	{
		for(int c = 0; c < MAX_X; c++)
		{
			level->visible[r][c] = 0;
		}
	}

	if (py >= 0 && py < MAX_Y && px >= 0 && px < MAX_X)
	{
		level->visible[py][px] = 1;
		level->discovered[py][px] = 1;
	}

	int min_r = py - radius; if (min_r < 0) min_r = 0;
	int max_r = py + radius; if (max_r >= MAX_Y) max_r = MAX_Y - 1;
	int min_c = px - radius; if (min_c < 0) min_c = 0;
	int max_c = px + radius; if (max_c >= MAX_X) max_c = MAX_X - 1;

	for(int c = min_c; c <= max_c; c++)
	{
		CastRay(level, py, px, min_r, c, radius);
		CastRay(level, py, px, max_r, c, radius);
	}
	for(int r = min_r; r <= max_r; r++)
	{
		CastRay(level, py, px, r, min_c, radius);
		CastRay(level, py, px, r, max_c, radius);
	}
}

void RenderFOV(Level *level)
{
	if(level == NULL || level->player == NULL || level->visible == NULL || level->discovered == NULL)
	{
		return;
	}

	int py = level->player->Position.yPos;
	int px = level->player->Position.xPos;

	for(int r = 0; r < MAX_Y; r++)
	{
		for(int c = 0; c < MAX_X; c++)
		{
			if (level->visible[r][c] == 1)
			{
				if (r == py && c == px)
				{
					PrintChar(r, c, level->player_symbol);
				}
				else
				{
					Entity *mon = GetMonsterAt(r, c, *level);
					if (mon != NULL && mon->alive == YES)
					{
						PrintChar(r, c, mon->symbol);
					}
					else
					{
						Corpse *corp = GetCorpseAt(*level, r, c);
						if (corp != NULL && corp->active == YES)
						{
							PrintChar(r, c, '%');
						}
						else
						{
							Item *it = GetItemAt(*level, r, c);
							if (it != NULL && it->active == YES)
							{
								PrintChar(r, c, it->symbol);
							}
							else
							{
								char ch = (level->map_backup != NULL) ? level->map_backup[r][c] : ' ';
								PrintChar(r, c, ch);
							}
						}
					}
				}
			}
			else if (level->discovered[r][c] == 1)
			{
				// Explored memory: living monsters are hidden in the dark!
				char ch = (level->map_backup != NULL) ? level->map_backup[r][c] : ' ';
				PrintChar(r, c, ch);
			}
			else
			{
				// Unexplored fog: black space
				PrintChar(r, c, ' ');
			}
		}
	}
	move(py, px);
}

