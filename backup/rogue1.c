// gcc --std=c99 -D_POSIX_C_SOURCE=199309L rogue.c -lncurses
// Headers
/*	char joku[12];
	sprintf(joku, "%d", ran);
	mvprintw(0,0,joku);
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

// Define global names:
#define FLOOR 0
#define DOOR 1
#define LOOT 2
#define ENEMY 3

// Variables
typedef struct Position
{
	int xPos;
	int yPos;
	int width;
	int height;
	int oldx;
	int oldy;
	int type;
	int c;
	char ch;
} Position;

typedef struct Entity
{
	Position Position;
	Position *Area;
	Position **Object;
	int i;
	int health;
	char input;
} Entity;

// Function names
int ScreenInit();
int RouteRooms(Position *doors1, Position *doors2, int i, int j);
int ConnectRooms(Position *doors1, Position *doors2, int d);
Position CheckArea(Position pos); 
Position CreateRoom(Position pos); 
Entity MapInit(Entity room); 
Position DrawDoor(int height, int width, Position pos, int i); 
Position *DrawRoom(Position pos, int d); 
int TurnNearby(int y, int x, int c, char *d); 
char **SaveMap(char **XY, int y, int x);
char **UpdateMap(char **map, int y, int x);
int HandleInput(Entity *player, char **map);
int PrintOld(int y, int x, char **map); 
int StepOn(int y, int x, char c, Entity *player, char **map);
int MovePlayer(int y, int x, Entity *player, char **map);

// Entity type structure pointer to a setup function with input as entity type pointer.
Entity *PlayerSetup(Entity *player, Entity room); 

// Functions
int main(void) 
{

// Variables
	int c = 0;
	Entity *player; 
	Entity rooms; 
	char **map; 
	int x = 80; int y = 40; 
	clock_t ct; ct = clock(); double clk = ct;
	srand(clk);
// Init
	ScreenInit();
	rooms = MapInit(rooms); 
	map = SaveMap(map,y,x);

	player = PlayerSetup(player, rooms); 

// MAIN LOOP
	while((c = getchar()) != 'Q') 
	{
		player->input = c; 
		HandleInput(player, map); 
	}
	
// Exit
	// Free allocations:
	free(player);
	free(rooms.Area);
	// Free the allocated door space!
	for(int i = 0; i<rooms.i; i++)
	{
		free(rooms.Object[i]); 
	}	
	free(rooms.Object); 
	// Free map screenshot
	for(int i=0; i<y; i++)
	{
		free(map[i]);
	}
	free(map);	

	endwin();
	return 0;
}

int ScreenInit()
{
	initscr();
	noecho(); 
	refresh(); 

	return 1;
}

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
{	// Width and height are one too long!
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

int RouteRooms(Position *doors1, Position *doors2, int i, int j)
{
	// Old positions
	int x1 = doors1[i].xPos; int y1 = doors1[i].yPos;
	int x2 = doors2[j].xPos; int y2 = doors2[j].yPos;
	int prex1=x1; int prey1=y1; 
	int prex2=x2; int prey2=y2;

	while(1) 
	{
			
		if(abs((x1-1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1-1))==' '))
		{ // Distance between step is smaller AND there is free space
			prex1 = x1; prex2 = x2;
			x1 = x1-1; 
		} else if(abs((x1-1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1-1))=='.'))
		{ // If distance smaller AND there is an open space
			prex1 = x1; prex2 = x2;
			x1 = x1-1;
		} else if(abs((x1+1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1+1))==' '))
		{
			prex1 = x1; prex2 = x2;
			x1 = x1+1;
		} else if(abs((x1+1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1+1))=='.'))
		{
			prex1 = x1; prex2 = x2;
			x1 = x1+1; 
		} else if(abs((y1-1)-y2)<abs(y1-y2)&&(mvinch((y1-1),x1)==' '))
		{
			prey1 = y1; prey2 = y2;
			y1 = y1-1;
		} else if(abs((y1-1)-y2)<abs(y1-y2)&&(mvinch((y1-1),x1)=='.'))
		{
			prey1 = y1; prey2 = y2;
			y1 = y1-1;
		} else if(abs((y1+1)-y2)<abs(y1-y2)&&(mvinch((y1+1),x1)==' '))
		{
			prey1 = y1; prey2 = y2;
			y1 = y1+1; 
		} else if(abs((y1+1)-y2)<abs(y1-y2)&&(mvinch((y1+1),x1)=='.'))
		{
			prey1 = y1; prey2 = y2;
			y1 = y1+1; 
		} else 
		{
			return 0; 
		}
		mvprintw(y1,x1,"."); // Print free space
	}	
	return 1;
}

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

	RouteRooms(doors1,doors2,I,J);

	return 1;
}

Position CheckArea(Position pos)
{
	int x,y,w,h=2;
	x=pos.xPos; y=pos.yPos; w=pos.width; h=pos.height;

	for(int i=(y-1); i<((h+1)+y); i++)
	{
		for(int j=(x-1); j<((w+1)+x); j++)
		{
			if(mvinch(i,j)==' ')
			{
				pos.type = 1;
			}
			else 
			{
				pos.type = 0;
				return pos; 
			}
		}
	}

	pos.xPos=x; pos.yPos=y;
	return pos;
}

Position CreateRoom(Position pos)
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
	room.i = (r+1); 

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
			room.Area[rooms] = CreateRoom(room.Area[rooms]);

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

char **SaveMap(char **XY, int y, int x)
{
	
	XY = malloc(sizeof(char*)*y); // Y-coordinate allocating

	for(int i=0; i<y; i++)
	{
		XY[i] = malloc(sizeof(char)*x); // X-coordinate allocating
		for(int j=0; j<x; j++)
		{
			XY[i][j]=mvinch(i,j);	
		}
	}
	return XY;
}

char **UpdateMap(char **map, int y, int x)
{
	for(int i=0; i<y; i++)
	{
		for(int j=0; j<x; j++)
		{
			map[i][j]=mvinch(i,j);	
		}
	}
	return map;
}

Entity *PlayerSetup(Entity *player, Entity room)
{
	player = malloc(sizeof(Entity));

	// Set original position for the player
	player->Position.xPos = (room.Area[0].xPos)+1; 
	player->Position.yPos = (room.Area[0].yPos)+1;
	player->health = 20; player->Position.type = FLOOR;

	// Print player and update the cursor
	mvprintw(player->Position.yPos, player->Position.xPos, "@");
	move(player->Position.yPos, player->Position.xPos);
	
	// WELCOME TEXT + INSTRUCTIONS:
	mvprintw(0,0, "Welcome! Press 'Q' to exit and move with wasd+qezx");
	refresh(); mvprintw(0,0, "\n"); // Clear the line
	getchar();

	return player;
}

int HandleInput(Entity *player, char **map)
{
	// Save the original position values:
	int y = player->Position.yPos;
	int x = player->Position.xPos;

	player->Position.oldy = y; 
	player->Position.oldx = x;

	switch(player->input)
	{	// Move up, update the position variable accordingly:
		case 'w':
			y = player->Position.yPos-1;
			x = player->Position.xPos;
		break;
		// Move down
		case 's':
			y = player->Position.yPos+1;
			x = player->Position.xPos;
		break;
		// Move back
		case 'a':
			y = player->Position.yPos;
			x = player->Position.xPos-1;
		break;
		// Move forward
		case 'd':
			y = player->Position.yPos;
			x = player->Position.xPos+1;
		break;
		// Move upleft
		case 'q':
			y = player->Position.yPos-1;
			x = player->Position.xPos-1;
		break;
		// Move upright
		case 'e':
			y = player->Position.yPos-1;
			x = player->Position.xPos+1;
		break;
		// Move downleft
		case 'z':
			y = player->Position.yPos+1;
			x = player->Position.xPos-1;
		break;
		// Move downright
		case 'x':
			y = player->Position.yPos+1;
			x = player->Position.xPos+1;
		break;

		// Close door:
		case 'c': 
			TurnNearby(y,x,':',"+");
		break;
		
		default:
		break;
	}

// After updating the position variable,first get the next character:	
	player->Position.c = mvinch(y,x);
	MovePlayer(y,x, player, map); 
	
	refresh();
	return 1;
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

int StepOn(int y, int x,char c, Entity *player, char **map)
{
	switch(c)
	{
	case '.': // If there is free space '.' then print player there
		PrintOld(player->Position.yPos, player->Position.xPos, map);
		UpdateMap(map,y,x); 
		
		player->Position.yPos=y; player->Position.xPos=x;
		mvprintw(player->Position.yPos, player->Position.xPos, "@");
		move(player->Position.yPos, player->Position.xPos);
	break;

	case ':': // If there is an open door ':' then print player there
		mvprintw(player->Position.yPos, player->Position.xPos, ":");
		player->Position.yPos=y; player->Position.xPos=x;
		mvprintw(player->Position.yPos, player->Position.xPos, "@");
		move(player->Position.yPos, player->Position.xPos);
	break;

	case '+': // If there is a closed door:
		mvprintw(0,0, "Do you want to open the door? y/n");
		refresh(); mvprintw(0,0, "\n");
		player->input = getchar();
		if(player->input=='y'||player->input=='Y') 
		{
			mvprintw(0,0, "You opened the door! (close a door with 'c')");
			refresh(); mvprintw(0,0, "\n"); getchar();
			TurnNearby(player->Position.yPos,player->Position.xPos,'+',":");
			move(player->Position.yPos, player->Position.xPos);
		}	
		else // Else just update cursor
		{
			move(player->Position.yPos, player->Position.xPos);
		}	

	// .. more here such as enemies and loot!

	default:
		
	break;
	}	
	return 1;
}

int MovePlayer(int y, int x, Entity *player, char **map)
{
	switch(player->Position.c)
	{
		case '.':
			if(player->Position.type==FLOOR)
			{
				StepOn(y,x,'.',player, map);
				player->Position.type=FLOOR;
			}
			else if(player->Position.type==DOOR)
			{
				StepOn(y,x,':',player, map);
				player->Position.type=FLOOR; 
			}
		break;
		
		case ':': 
			StepOn(y,x,'.',player, map);
			player->Position.type=DOOR;
		break;
		
		case '+': 
			StepOn(y,x,'+',player, map);
			player->Position.type=FLOOR; 
		break;

		default:
			move(player->Position.yPos, player->Position.xPos);
		break;
	}
	return 1;
}

