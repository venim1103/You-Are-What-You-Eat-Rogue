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

typedef struct Corpse
{
	Position Position;
	char symbol;
	char *name;
	int maxhp;
	int attack;
	int nutrition;
	int age;
	int active;
} Corpse;

typedef struct Item
{
	Position Position;
	char symbol;       // ')' for weapon, '[' for armor, '%' for corpse
	char *name;
	int bonus_att;
	int bonus_def;
	int item_type;     // ITEM_WEAPON, ITEM_ARMOR, ITEM_CORPSE
	int equipped;      // YES / NO
	int active;        // On floor? YES / NO
	Corpse corpse_data;
} Item;

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
	int ability_cd;
	int ability_max_cd;
	int ability_cost;
	char *ability_name;
	char *trait_desc;
	Item inventory[10];
	int num_items;
	int equipped_weapon; // -1 if none
	int equipped_armor;  // -1 if none
	char input;
	char symbol;
	char *name;
} Entity;

typedef struct Level
{
	Entity rooms;
	Entity *player;
	struct Entity **monsters;
	struct Corpse *corpses;
	struct Item *items;
	char player_symbol;
	char *killer;
	char **map;
	char **map_backup;
	char **discovered;
	char **visible;
	int lvl;
	int maxlvl;
	int mons;
	int max_mons;
	int turn;
	int num_corpses;
	int max_corpses;
	int num_items;
	int max_items;
} Level;

#endif // TO END THE DEFINITION FUNCTION TO ONE TIME USE ONLY!
