#include "rogue.h"

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

int TurnNearby(int y, int x, int c, char *d)
{
	if(mvinch(y-1,x)==c)/*->*/mvprintw(y-1,x,d); // above
	if(mvinch(y+1,x)==c)/*->*/mvprintw(y+1,x,d); // below
	if(mvinch(y,x-1)==c)/*->*/mvprintw(y,x-1,d); // left
	if(mvinch(y,x+1)==c)/*->*/mvprintw(y,x+1,d); // right
	
	if(mvinch(y-1,x+1)==c)/*->*/mvprintw(y-1,x+1,d); //upright
	if(mvinch(y-1,x-1)==c)/*->*/mvprintw(y-1,x-1,d); //upleft
	if(mvinch(y+1,x+1)==c)/*->*/mvprintw(y+1,x+1,d); //downright
	if(mvinch(y+1,x-1)==c)/*->*/mvprintw(y+1,x-1,d); //downleft
	return 1;
}

int PrintOld(int y, int x, char **map)
{
	char buf[8]; 
	sprintf(buf, "%c", map[y][x]);
	mvprintw(y,x,buf);
	return 1;
}

