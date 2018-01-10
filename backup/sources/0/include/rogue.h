// TO AVOID REUSING THIS CODE TO MAKE NEW VARIABLES EACH TIME:
#ifndef ROGUE_H
#define ROGUE_H

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

// Define global types:
//		0	1	2	3
typedef enum { FLOOR, DOOR, LOOT, ENEMY } TileType; 

#define MAX_X 80
#define MAX_Y 40

// Variables
typedef struct Position
{
	int xPos;
	int yPos;
	int width;
	int height;
//	int oldx;
//	int oldy;
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
Position CreateRoomSize(Position pos); 
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

#endif // TO END THE DEFINITION FUNCTION TO ONE TIME USE ONLY!
