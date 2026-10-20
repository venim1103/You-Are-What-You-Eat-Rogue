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
	int hunger;
	int max_hunger;
	int hunger_rate;
	int poison_turns;
	int regen_rate;
	int can_open_doors;
	int extra_attack_chance;
	int poison_attack;
	int cleave_chance;
	int scavenger_bonus;
	int can_phase_walls;
	int acid_corrosion;
	int evasion_chance;
	int life_drain;
	char *trait_desc;
	char input;
	char symbol;
	char *name;
} Entity;

typedef struct Corpse
{
	Position Position;
	char symbol;
	char *name;
	int maxhp;
	int attack;
	int nutrition;
	int active;
} Corpse;

typedef struct Level
{
	Entity rooms;
	Entity *player;
	struct Entity **monsters;
	struct Corpse *corpses;
	char player_symbol;
	char *killer;
	char **map;
	char **map_backup;
	int lvl;
	int maxlvl;
	int mons;
	int turn;
	int num_corpses;
	int max_corpses;
} Level;

#endif // TO END THE DEFINITION FUNCTION TO ONE TIME USE ONLY!
