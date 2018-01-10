// TO AVOID REUSING THIS CODE TO MAKE NEW VARIABLES EACH TIME:
#ifndef STRUCTS_H
#define STRUCTS_H

// Variables
typedef struct Position
{
	int xPos;
	int yPos;
	int width;
	int height;
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

typedef struct Level
{
	Entity rooms;
	Entity *player;
	char **map;
	int lvl;
} Level;

#endif // TO END THE DEFINITION FUNCTION TO ONE TIME USE ONLY!
