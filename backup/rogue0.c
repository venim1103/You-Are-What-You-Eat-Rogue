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
Position CheckArea(Position pos); // Function for checking empty area on level
Position CreateRoom(Position pos); // Create a room of number r
Entity MapInit(Entity room); // Entity for room with position type array output.
Position DrawDoor(int height, int width, Position pos, int i); //Draw a single door
Position *DrawRoom(Position pos, int d); // Draw room with doors
int TurnNearby(int y, int x, int c, char *d); //Turn nearby characters to others
char **SaveMap(char **XY, int y, int x); // Take a screenshot of the whole map
char **UpdateMap(char **map, int y, int x);
int HandleInput(Entity *player, char **map);
int PrintOld(int y, int x, char **map); //Get the character from below )
int StepOn(int y, int x, char c, Entity *player, char **map); // Moving player
int MovePlayer(int y, int x, Entity *player, char **map);

// Entity type structure pointer to a setup function with input as entity type pointer.
Entity *PlayerSetup(Entity *player, Entity room); // The functions needs to return entity type pointer for free!

// Functions
int main(void) //int argc char **argv
{

// Variables
	int c = 0;
	Entity *player; //pointer to player.
	Entity rooms; // Rooms entity.
	char **map; //Pointer for map screenshot
	int x = 80; int y = 40; // Size of the map!
	clock_t ct; ct = clock(); double clk = ct;
	srand(clk); // Generate random with CPU clock!
// Init
	ScreenInit();
	rooms = MapInit(rooms); // Initialize rooms and update the main variable
	map = SaveMap(map,y,x); // Take a screenshot of the current level

	player = PlayerSetup(player, rooms); // Initialize player and update main variable

// MAIN LOOP
	while((c = getchar()) != 'Q') // All the time check q for quitting.
	{
		player->input = c; // Store the input character
		HandleInput(player, map); // Handle the input
	}
	
// Exit
	// Free allocations:
	free(player);
	free(rooms.Area);
	// Free the allocated door space!
	for(int i = 0; i<rooms.i; i++)
	{
		free(rooms.Object[i]); // Free individual doors
	}	
	free(rooms.Object); // Free the door connected rooms
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
	initscr(); // Initialize the screen.
	noecho(); // Disable stdin echoing
	refresh(); // Init refresh

	return 1;
}

Position DrawDoor(int height, int width, Position pos, int i)
{
	int ranw = (rand() % (width-1))+1; // random number from 0to(width-1) thus requires to add +1 at the end to get between 1to(width-1), as no need for 2 corners the width has to become width-2 -> (originally width=width-1 so width-1=width-2)!
	int ranh = (rand() % (height-1))+1; // random number from 0to(width-1) thus requires to add +1 at the end to get between 1to(width-1), as no need for 2 corners the width has to become width-2 -> (originally width=width-1 so width-1=width-2)!

	Position door; // Variable for position

	// Print door
	switch(i) // Number for sides
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
	int height = pos.height-1;
	int width = pos.width-1;

// Print walls and floor:
	// Top and bottom with - (1 more wide) 
	for(int i = 0; i < width+1; i++)
	{	
		mvprintw(pos.yPos ,pos.xPos+i, "-");
		mvprintw(pos.yPos+height, pos.xPos+i, "-");
	}	
	// Print the rest (because not top or bottom = ypos+1->height
	for(int i = 1; i < height; i++)
	{	// Print left corner with |
		mvprintw(pos.yPos+i, pos.xPos, "|");
		// Print right corner with |
		mvprintw(pos.yPos+i, pos.xPos+width, "|");
		// Print the middle part with .
		for(int j = 1; j < width; j++)
		{	// again starting from xpos+1 to width-1
			mvprintw(pos.yPos+i, pos.xPos+j, ".");
		}
	}
// Print doors:

	// Draw the doors:
	Position *doors;
	doors = malloc(sizeof(Position)*d); // Allocate space for doors

	for(int i=0;i<d;i++) // four sides of doors
	{
		doors[i] = DrawDoor(height,width,pos,i);
	}

	return doors;	
}

int RouteRooms(Position *doors1, Position *doors2, int i, int j)
{
	// Old positions
	int x1 = doors1[i].xPos; int y1 = doors1[i].yPos; // doors1
	int x2 = doors2[j].xPos; int y2 = doors2[j].yPos; // doors2
	int prex1=x1; int prey1=y1; // Previous values
	int prex2=x2; int prey2=y2;

	while(1) // Wait until all options completed
	{
			// Step left
		if(abs((x1-1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1-1))==' '))
		{ // Distance between step is smaller AND there is free space
			prex1 = x1; prex2 = x2;
			x1 = x1-1; // "Step left 1"
		} else if(abs((x1-1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1-1))=='.'))
		{ // If distance smaller AND there is an open space
			prex1 = x1; prex2 = x2;
			x1 = x1-1; // "Step left 2"
		} else if(abs((x1+1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1+1))==' '))
		{
			prex1 = x1; prex2 = x2;
			x1 = x1+1; // "Step right 1"
		} else if(abs((x1+1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1+1))=='.'))
		{
			prex1 = x1; prex2 = x2;
			x1 = x1+1; // "Step right 2"
		} else if(abs((y1-1)-y2)<abs(y1-y2)&&(mvinch((y1-1),x1)==' '))
		{
			prey1 = y1; prey2 = y2;
			y1 = y1-1; // "Step down 1"
		} else if(abs((y1-1)-y2)<abs(y1-y2)&&(mvinch((y1-1),x1)=='.'))
		{
			prey1 = y1; prey2 = y2;
			y1 = y1-1; // "Step down 2"
		} else if(abs((y1+1)-y2)<abs(y1-y2)&&(mvinch((y1+1),x1)==' '))
		{
			prey1 = y1; prey2 = y2;
			y1 = y1+1; // "Step up 1"
		} else if(abs((y1+1)-y2)<abs(y1-y2)&&(mvinch((y1+1),x1)=='.'))
		{
			prey1 = y1; prey2 = y2;
			y1 = y1+1; // "Step up 2"
		} else 
		{
			return 0; // END LOOP
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
//	mvprintw(doors1[I].yPos, doors1[I].xPos, "1");
//	mvprintw(doors2[J].yPos, doors2[J].xPos, "2");

	RouteRooms(doors1,doors2,I,J);

	return 1;
}

Position CheckArea(Position pos)
{
	int x,y,w,h=2;
	x=pos.xPos; y=pos.yPos; w=pos.width; h=pos.height;

	// Safety procedure
	if(y<2)
	{
		y=2;		
	}	
	if(x<2)
	{
		x=2;		
	}	

	for(int i=(y-1); i<((h+1)+y); i++)
	{
		for(int j=(x-1); j<((w+1)+x); j++)
		{
			if(mvinch(i,j)==' ')
			{
//				getch();
				pos.type = 1;
			// ON SUCCESS mvprintw(i,j,"f"); // Print on free space
				// FOR TESTING!!!!!!!!!!!!
//				mvprintw(i,j,"f");
			}
			else 
			{
				pos.type = 0;
				pos.xPos=pos.oldx; pos.yPos=pos.oldy;
				return pos; // ELSE FAIL! // mvprintw(i,j,"#");
			//	break; 
			}
		}
	}

	pos.xPos=x; pos.yPos=y;
	return pos;
}

Position CreateRoom(Position pos)
{
	int ran = 3; ran = (rand()+3); // Random number	
	
	// Save old values 
	pos.oldx = pos.xPos; pos.oldy = pos.yPos;

	// Max values for position and height+width
	int x,y=1; 
	int w,h=2; int xmax = 5; int ymax = 5; int wmax = 20; int hmax = 8;

	x = pos.xPos; y = pos.yPos;
//	x = (((pos.xPos+ran)%xmax)); // Place room between last coord + random
//	y = (((pos.yPos+ran)%ymax)); // Place room between last coord + random
	h = ((ran*100+x+y)%hmax+2); // Set random sizes for the room
	w = ((ran*100+x+y)%wmax+2); // Add one to make sure the size will be atleast 2!

//	pos.xPos=x; pos.yPos=y; 
	pos.height=h; pos.width=w;

	// Test the area for free space:
	pos = CheckArea(pos);


// FOR DEBUGGING!
	getch();
	char buf[30];
	sprintf(buf, "x=%d y=%d",pos.xPos,pos.yPos);
	mvprintw(0,0,buf);
	getch();

	return pos;
}

Entity MapInit(Entity room)
{
	int r = 5; // Number of rooms
	int d = 4; // Number of doors = 4 because of each side one
	room.i = r; 
	// room.Area == all the rooms	room.Object == all doors in all rooms

	room.Area = malloc(sizeof(Position)*r); // Allocate the space for the rooms
	room.Object = malloc(sizeof(Position)*d*r); // Allocate space rooms+doors

// CREATE ROOMS:
	int x = 1; int y = 1; // Initial starting position for first room

	for(int i=0; i<r; i++)
	{
		room.Area[i].xPos=x; room.Area[i].yPos=y;
		room.Area[i].oldx=x; room.Area[i].oldy=y;
	}

	int rooms = 0; 
//	for(int i=0; i<10; i++) // Do for all the rooms
//	{
	while(((room.Area[rooms].yPos + room.Area[rooms].height)+1)<39)
	{
		if((room.Area[rooms].xPos+room.Area[rooms].width+20)<79)
		{

		// For DEBUGGING:
			getch();	
			char buf[30];
			sprintf(buf, "X=%d Y=%d",room.Area[rooms].xPos,room.Area[rooms].yPos);
			mvprintw(0,0,buf);
			getch();


			room.Area[rooms] = CreateRoom(room.Area[rooms]);

			if(room.Area[rooms].type==1) // When the space checking works!
			{
				mvprintw(0,0,"HERE!");
				getch();
			
				room.Object[rooms] = DrawRoom(room.Area[rooms],d); // Draw room!
				rooms++;
			}// else {if(i>0){i = i-1;}} // Try again!
			if(rooms>r) 
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
		if(rooms>r) 
		{
			break;
		}
	}



//	int x = 5; int y = 3; int w = 12; int h = 10;
//	room.Area[0].xPos=x; room.Area[0].yPos=y; room.Area[0].width=w; room.Area[0].height=h;
//	room.Object[0] = DrawRoom(room.Area[0],d); // Save door positions	


	// Testing the CheckArea function:
/*	Position testi;
	int is = 0;
	x = 5; y=20; h=10; w=10;
	testi.xPos=x; testi.yPos=y; testi.height=h; testi.width=w;   
	is = CheckArea(testi);
	char output[8];
	sprintf(output, "%d", is);	
	mvprintw(1,0,output);
*/

// Connect all the rooms together
	for(int i=0; i<rooms; i++)
	{
		for(int j=0; j<rooms; j++)
		{
			ConnectRooms(room.Object[i], room.Object[j], d);
		}
	}
	return room; 
}

char **SaveMap(char **XY, int y, int x)
{
//	char **XY == Pointer for XY matrix
	
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
	player = malloc(sizeof(Entity)); // Allocate the space for the pointer

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

	player->Position.oldy = y; // Save for later use	
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
		case 'c': // If different directions have a door nearby: 
			TurnNearby(y,x,':',"+");
		break;
		
		default:
		break;
	}

// After updating the position variable,first get the next character:	
	player->Position.c = mvinch(y,x);
	MovePlayer(y,x, player, map); // Then move the player accordingly.
	
	refresh();
	return 1;
}

int TurnNearby(int y, int x, int c, char *d)
{
	// mvprint requires a string variable char *c ---> "" instead of ''!
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
	char buf[8]; // Buffer for the character
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
		
		UpdateMap(map,y,x); // Update the map after getting old char
		
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
		refresh(); mvprintw(0,0, "\n"); // Clear the line
		player->input = getchar();
		if(player->input=='y'||player->input=='Y') // If andswered yes
		{
			mvprintw(0,0, "You opened the door! (close a door with 'c')");
			refresh(); mvprintw(0,0, "\n"); getchar();// Clear the line
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
	// If the next character is '.' and the last movement type was "FLOOR"
			if(player->Position.type==FLOOR)
			{
				StepOn(y,x,'.',player, map);
				player->Position.type=FLOOR; // Still on floor
			}
	// If the next character is '.' but underneath is a DOOR (last type "DOOR")
			else if(player->Position.type==DOOR)
			{
				StepOn(y,x,':',player, map);
				player->Position.type=FLOOR; // Back on floor.
			}
		break;
		
		case ':': // If moved into an open door '.', act empty space '.'
			StepOn(y,x,'.',player, map);
			player->Position.type=DOOR; // Step on door update type
		break;
		
		case '+': // If moved into a closed  door '+', act closed door '+'
			StepOn(y,x,'+',player, map);
			player->Position.type=FLOOR; // Open door update type
		break;

	// Otherwise just update and move the cursor back to player position.
		default:
			move(player->Position.yPos, player->Position.xPos);
		break;
	}
	return 1;
}

