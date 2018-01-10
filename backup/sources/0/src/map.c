#include "rogue.h"

Position CreateRoomSize(Position pos)
{
	int ranh,ranw = 3; 

	// Max values for position and height+width
	int x,y=1; 
	int w,h=2; int wmax = 20; int hmax = 8;
	
	ranh = (rand()%hmax)+1; ranw = (rand()%wmax)+1; 
	
	x = pos.xPos; y = pos.yPos;
	h = (((x+y)+ranh)%hmax)+2;
	w = (((x+y)+ranw)%wmax)+2;

	pos.height=h; pos.width=w;

	// Test the area for free space:
	pos = CheckArea(pos);

	return pos;
}

Entity MapInit(Entity room)
{
	int r = 12; // Number of rooms
	int d = 4; // Number of doors = 4 because of each side one
	room.i = (r+1); // SET ONE MORE FOR MORE MEMORY! 

	room.Area = malloc(sizeof(Position)*room.i); 
	room.Object = malloc(sizeof(Position)*d*room.i); 

// CREATE ROOMS:
	// Initialize all the values to non-zero and non-random
	int x = 1; int y = 1; int w = 2; int h = 2;  
	for(int i=0; i<room.i; i++)
	{
		room.Area[i].xPos=x; room.Area[i].yPos=y;
		room.Area[i].oldx=x; room.Area[i].oldy=y;
		room.Area[i].height=h; room.Area[i].width=w; 
	}

	int rooms = 0; 
	while(((room.Area[rooms].yPos + room.Area[rooms].height)+1)<40)
	{
		if((room.Area[rooms].xPos+room.Area[rooms].width+20)<80)
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
			room.Area[rooms].yPos = (room.Area[rooms].yPos+5);
		}
		if(rooms>=r) 
		{
			break;
		}
	}
	room.i = rooms; 

// Connect all the rooms together
	for(int i=0; i<room.i; i++)
	{
		for(int j=0; j<room.i; j++)
		{
			ConnectRooms(room.Object[i], room.Object[j], d);
		}
	}
	return room; 
}

