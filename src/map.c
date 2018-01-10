#include "global.h"

Position CreateRoomSize(Position pos)
{
	// The width w and height h are always 1 too small while the empty space is always 1 too big!

	int ranh,ranw = 3; 

	// Max values for position and height+width
	int x,y=1; 
	int w,h=3; int wmax = 21; int hmax = 10;
	
	ranh = (rand()%hmax)+1; ranw = (rand()%wmax)+1; 
	
	x = pos.xPos; y = pos.yPos;
	h = (((x+y)+ranh)%hmax)+3; // +3 to make sure not too small!
	w = (((x+y)+ranw)%wmax)+3;

	pos.height=h; pos.width=w;

	// Test the area for free space:
	pos = CheckArea(pos, ' ');

	return pos;
}

Entity MapInit(Entity room)
{
	int r = ROOMS; // Number of rooms
	int d = 4; // Number of doors = 4 because of each side one
	room.mode = (r+1); // SET ONE MORE FOR MORE MEMORY! 

	room.Area = malloc(sizeof(Position)*room.mode); 
	room.Object = malloc(sizeof(Position)*d*room.mode); 

// CREATE ROOMS:
	// Initialize all the values to non-zero and non-random
	int x = 1; int y = 1; int w = 3; int h = 3;  
	for(int i=0; i<room.mode; i++)
	{
		room.Area[i].xPos=x; room.Area[i].yPos=y;
		room.Area[i].height=h; room.Area[i].width=w; 
	}

	int rooms = 0; 
	while(((room.Area[rooms].yPos + room.Area[rooms].height)+2)<(MAX_Y-2))
	{
		if((room.Area[rooms].xPos+room.Area[rooms].width+20)<MAX_X)
		{
			room.Area[rooms] = CreateRoomSize(room.Area[rooms]);

			if(room.Area[rooms].type==1)
			{
				room.Object[rooms] = DrawRoom(room.Area[rooms],d); 
				rooms = rooms+1;
			}
			if(rooms>=r) 
			{
				break;
			}
			room.Area[rooms].xPos=(room.Area[rooms].xPos+10);
		}
		else
		{
			room.Area[rooms].xPos = 1;
			room.Area[rooms].yPos = (room.Area[rooms].yPos+3);
		}
		if(rooms>=r) 
		{
			break;
		}
	}
	room.mode = rooms; 

// Connect all the rooms together
	for(int i=0; i<room.mode; i++)
	{
		for(int j=0; j<room.mode; j++)
		{
			ConnectRooms(room.Object[i], room.Object[j], d);
		}
	}

	return room; 
}

