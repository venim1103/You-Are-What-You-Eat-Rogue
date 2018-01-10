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
	int mode;
	int type;
	int health;
	int maxhp;
	int attack;
	int xp;
	int alive;
	char input;
	char symbol;
	char *name;
} Entity;

typedef struct Level
{
	Entity rooms;
	Entity *player;
	struct Entity **monsters;
	char player_symbol;
	char *killer;
	char **map;
	char **map_backup;
	int lvl;
	int maxlvl;
	int mons;
} Level;

#endif // TO END THE DEFINITION FUNCTION TO ONE TIME USE ONLY!
