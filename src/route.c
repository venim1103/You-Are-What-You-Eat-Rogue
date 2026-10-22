#include "global.h"

int ConnectRooms(Position *doors1, Position *doors2, int d)
{
	int dist = 9999; int I=0; int J=0;
	// Check which doors are the closest to each other:
	for(int i = 0; i<d; i++)
	{
		for(int j = 0; j<d; j++)
		{	// If previous distance is bigger than new distance:
			if(dist > (abs(doors1[i].xPos-doors2[j].xPos)+abs(doors1[i].yPos-doors2[j].yPos)))
			{
				I = i; J = j; // Save the marking numbers 	
				dist = abs(doors1[i].xPos-doors2[j].xPos)+abs(doors1[i].yPos-doors2[j].yPos);
			} else dist=dist;
		}
	}

	Route(doors1,doors2,I,J, ' ', '#', '#', YES);

	return 1;
}

int Route(Position *p1, Position *p2, int i, int j, char chk1, char chk2, char in, int mode)
{
	int x1,x2,y1,y2 = 0;
	x1 = p1[i].xPos; y1 = p1[i].yPos;
	x2 = p2[j].xPos; y2 = p2[j].yPos;

	char buf[8];
	sprintf(buf,"%c",in);

	while(1) 
	{
			
	if(abs((x1-1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1-1))==chk1))
		{ // Distance between step smaller AND there is free space
			x1 = x1-1; 
	} else if(abs((x1-1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1-1))==chk2))
		{ // If distance smaller AND there is an open space
			x1 = x1-1;
	} else if(abs((x1+1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1+1))==chk1))
		{
			x1 = x1+1;
	} else if(abs((x1+1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1+1))==chk2))
		{
			x1 = x1+1; 
	} else if(abs((y1-1)-y2)<abs(y1-y2)&&(mvinch((y1-1),x1)==chk1))
		{
			y1 = y1-1;
	} else if(abs((y1-1)-y2)<abs(y1-y2)&&(mvinch((y1-1),x1)==chk2))
		{
			y1 = y1-1;
	} else if(abs((y1+1)-y2)<abs(y1-y2)&&(mvinch((y1+1),x1)==chk1))
		{
			y1 = y1+1; 
	} else if(abs((y1+1)-y2)<abs(y1-y2)&&(mvinch((y1+1),x1)==chk2))
		{
			y1 = y1+1; 
		} else 
		{
			return 0; 
		}
	
	if(mode==YES)
	{
		mvprintw(y1,x1,"%s",buf); // Print free space
	}
	
	}	
	return 1;
}
int *PathFindFollow(int *yx, int y1, int x1, int y2, int x2, char **map, int can_phase)
{
	int best_dist = abs(y1 - y2) + abs(x1 - x2);
	int best_y = y1;
	int best_x = x1;

	// Check 4 cardinal directions:
	int dy[4] = {-1, 1, 0, 0};
	int dx[4] = {0, 0, -1, 1};

	for(int i = 0; i < 4; i++)
	{
		int ny = y1 + dy[i];
		int nx = x1 + dx[i];
		int d = abs(ny - y2) + abs(nx - x2);

		if(d < best_dist && ny > 0 && ny < MAX_Y - 1 && nx > 0 && nx < MAX_X - 1)
		{
			// Check if target is player or walkable terrain:
			if(ny == y2 && nx == x2)
			{
				best_dist = d;
				best_y = ny;
				best_x = nx;
			}
			else
			{
				chtype ch = (map != NULL) ? map[ny][nx] : '.';
				if(can_phase || ch == '.' || ch == '#' || ch == ':' || ch == '%' || ch == ')' || ch == '[' || ch == '<' || ch == '>')
				{
					best_dist = d;
					best_y = ny;
					best_x = nx;
				}
			}
		}
	}

	yx[0] = best_y;
	yx[1] = best_x;
	return yx;
}

int *PathFindRandom(int *yx, int y, int x, char **map, int mode)
{
	int random = 0;
	if(mode==RANDOM_S)
	{	
		random = (rand()%4);
	} else
	{
		random = (rand()%8);
	}

	int dy = 0, dx = 0;
	switch(random)
	{
		case UP:        dy = -1; dx = 0;  break;
		case DOWN:      dy =  1; dx = 0;  break;
		case LEFT:      dy =  0; dx = -1; break;
		case RIGHT:     dy =  0; dx =  1; break;
		case UPLEFT:    dy = -1; dx = -1; break;
		case UPRIGHT:   dy = -1; dx =  1; break;
		case DOWNLEFT:  dy =  1; dx = -1; break;
		case DOWNRIGHT: dy =  1; dx =  1; break;
		default: break;
	}

	int ny = y + dy;
	int nx = x + dx;
	if(ny >= 0 && ny < MAX_Y && nx >= 0 && nx < MAX_X)
	{
		chtype ch = (map != NULL) ? map[ny][nx] : '.';
		if(ch == '.' || ch == '#' || ch == ':' || ch == '%')
		{
			y = ny;
			x = nx;
		}
	}

	yx[0] = y;
	yx[1] = x;
	return yx;
}

