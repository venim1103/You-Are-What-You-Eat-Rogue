#include "global.h"

void ApplyFormTraits(Entity *entity, char symbol)
{
	switch(symbol)
	{
		case 'x': // Spider
			entity->hunger_rate = 2;
			entity->regen_rate = 0;
			entity->can_open_doors = NO;
			entity->extra_attack_chance = 40;
			entity->poison_attack = 0;
			entity->cleave_chance = 0;
			entity->scavenger_bonus = 0;
			entity->can_phase_walls = 0;
			entity->acid_corrosion = 0;
			entity->evasion_chance = 0;
			entity->life_drain = 0;
			entity->ability_name = "Spin Web";
			entity->ability_max_cd = 6;
			entity->ability_cost = 40;
			entity->trait_desc = "Skitter (40% 2nd hit), Fast Metabolism (2 hunger/t), No Hands";
			break;
		case 's': // Snake
			entity->hunger_rate = 1;
			entity->regen_rate = 0;
			entity->can_open_doors = NO;
			entity->extra_attack_chance = 0;
			entity->poison_attack = 1;
			entity->cleave_chance = 0;
			entity->scavenger_bonus = 0;
			entity->can_phase_walls = 0;
			entity->acid_corrosion = 0;
			entity->evasion_chance = 0;
			entity->life_drain = 0;
			entity->ability_name = "Venom Spit";
			entity->ability_max_cd = 5;
			entity->ability_cost = 30;
			entity->trait_desc = "Venomous Strike, Poison Immune, No Hands";
			break;
		case 'g': // Goblin
			entity->hunger_rate = 1;
			entity->regen_rate = 0;
			entity->can_open_doors = YES;
			entity->extra_attack_chance = 0;
			entity->poison_attack = 0;
			entity->cleave_chance = 0;
			entity->scavenger_bonus = 50;
			entity->can_phase_walls = 0;
			entity->acid_corrosion = 0;
			entity->evasion_chance = 0;
			entity->life_drain = 0;
			entity->ability_name = "Flashbang";
			entity->ability_max_cd = 8;
			entity->ability_cost = 35;
			entity->trait_desc = "Scavenger (+50% corpse nutrition)";
			break;
		case 'T': // Troll
			entity->hunger_rate = 3;
			entity->regen_rate = 2;
			entity->can_open_doors = YES;
			entity->extra_attack_chance = 0;
			entity->poison_attack = 0;
			entity->cleave_chance = 35;
			entity->scavenger_bonus = 0;
			entity->can_phase_walls = 0;
			entity->acid_corrosion = 0;
			entity->evasion_chance = 0;
			entity->life_drain = 0;
			entity->ability_name = "Ground Slam";
			entity->ability_max_cd = 8;
			entity->ability_cost = 60;
			entity->trait_desc = "Regen (1HP/2t), Crushing Cleave, Gluttony (3 hunger/t)";
			break;
		case 'G': // Ghost
			entity->hunger_rate = 2;
			entity->regen_rate = 0;
			entity->can_open_doors = NO;
			entity->extra_attack_chance = 0;
			entity->poison_attack = 0;
			entity->cleave_chance = 0;
			entity->scavenger_bonus = 0;
			entity->can_phase_walls = 1;
			entity->acid_corrosion = 0;
			entity->evasion_chance = 25;
			entity->life_drain = 0;
			entity->ability_name = "Shadow Blink";
			entity->ability_max_cd = 7;
			entity->ability_cost = 50;
			entity->trait_desc = "Incorporeal (Phases through walls/doors), 25% Evasion, No Hands";
			break;
		case 'B': // Slime / Acid Blob
			entity->hunger_rate = 1;
			entity->regen_rate = 0;
			entity->can_open_doors = NO;
			entity->extra_attack_chance = 0;
			entity->poison_attack = 0;
			entity->cleave_chance = 0;
			entity->scavenger_bonus = 0;
			entity->can_phase_walls = 0;
			entity->acid_corrosion = 1;
			entity->evasion_chance = 0;
			entity->life_drain = 0;
			entity->ability_name = "Acid Jet";
			entity->ability_max_cd = 5;
			entity->ability_cost = 45;
			entity->trait_desc = "Caustic Body (Reflects acid dmg on hit/attacker), Poison Immune, No Hands";
			break;
		case 'V': // Vampire Bat
			entity->hunger_rate = 2;
			entity->regen_rate = 0;
			entity->can_open_doors = NO;
			entity->extra_attack_chance = 0;
			entity->poison_attack = 0;
			entity->cleave_chance = 0;
			entity->scavenger_bonus = 0;
			entity->can_phase_walls = 0;
			entity->acid_corrosion = 0;
			entity->evasion_chance = 50;
			entity->life_drain = 1;
			entity->ability_name = "Echo Screech";
			entity->ability_max_cd = 6;
			entity->ability_cost = 30;
			entity->trait_desc = "Flight (50% Evasion), Life Drain (+1 HP on hit), Fast Burn (2 hunger/t), No Hands";
			break;
		case '@': // Human
		default:
			entity->hunger_rate = 1;
			entity->regen_rate = 0;
			entity->can_open_doors = YES;
			entity->extra_attack_chance = 0;
			entity->poison_attack = 0;
			entity->cleave_chance = 0;
			entity->scavenger_bonus = 0;
			entity->can_phase_walls = 0;
			entity->acid_corrosion = 0;
			entity->evasion_chance = 0;
			entity->life_drain = 0;
			entity->ability_name = "Bandage";
			entity->ability_max_cd = 10;
			entity->ability_cost = 35;
			entity->trait_desc = "Balanced Human Form";
			break;
	}
}

void CopyPlayer(Entity *dest, Entity *src)
{
	dest->health = src->health;
	dest->maxhp = src->maxhp;
	dest->attack = src->attack;
	dest->xp = src->xp;
	dest->hunger = src->hunger;
	dest->max_hunger = src->max_hunger;
	dest->hunger_rate = src->hunger_rate;
	dest->poison_turns = src->poison_turns;
	dest->regen_rate = src->regen_rate;
	dest->can_open_doors = src->can_open_doors;
	dest->extra_attack_chance = src->extra_attack_chance;
	dest->poison_attack = src->poison_attack;
	dest->cleave_chance = src->cleave_chance;
	dest->scavenger_bonus = src->scavenger_bonus;
	dest->can_phase_walls = src->can_phase_walls;
	dest->acid_corrosion = src->acid_corrosion;
	dest->evasion_chance = src->evasion_chance;
	dest->life_drain = src->life_drain;
	dest->ability_cd = src->ability_cd;
	dest->ability_max_cd = src->ability_max_cd;
	dest->ability_cost = src->ability_cost;
	dest->ability_name = src->ability_name;
	dest->trait_desc = src->trait_desc;
	dest->name = src->name;
	dest->symbol = src->symbol;
	dest->Position.type = src->Position.type;
}

Entity *PlayerSetup(Level *leveli, Entity *player, Entity room)
{
	player = malloc(sizeof(Entity));

//	leveli->player_symbol = '@';
//	player->symbol = leveli->player_symbol;

	player->Position.type = UNKNOWN;

	// Take random room
	int randomroom = 0; randomroom = (rand()%room.mode);

	// Put somewhere randomly
	int *yx; int x,y=3; yx = malloc(sizeof(int)*3); // init random coords
	yx = RandomCoords(yx,room.mode,room.Area[randomroom]);
	y = yx[0]; x = yx[1];
	
	// FOR INITIAL MOVEMENT BUG:
	player->mode = 0;

	// Set original position for the player
	player->Position.xPos = x; player->Position.yPos = y;
	player->Position.ch = '>';
	player->health = 20;
	player->maxhp = player->health;
	player->attack = 1;
	player->xp = 0;
	player->hunger = 800;
	player->max_hunger = 1000;
	player->name = "Human";
	player->symbol = '@';
	player->alive = YES;
	player->poison_turns = 0;
	player->ability_cd = 0;
	ApplyFormTraits(player, '@');

	// Print upstairs and downstairs + update the cursor
	PrintChar(player->Position.yPos,player->Position.xPos,'>');
	int room_old = randomroom;

	randomroom = (rand()%room.mode); 
	while(randomroom==room_old)
	{
		randomroom = (rand()%room.mode); 
	}

	// Put somewhere random	
	yx = RandomCoords(yx,room.mode,room.Area[randomroom]);
	y = yx[0]; x = yx[1];
	PrintChar(y,x,'<');		
	
	move(player->Position.yPos,player->Position.xPos);
	
	free(yx); // free the used random location pointer
	return player;
}

Entity *PlayerMove(Entity *player, char *buf, char c, int nexty, int nextx)
{
	char ch[8];
	sprintf(ch,"%c",c);

        ///// FOR FIRST MOVEMENT BUG FIXING!!://////  
        if(player->mode==YES) // As long as the there was one movement:
        { // cause for the bug:   
                mvprintw(player->Position.yPos,player->Position.xPos, "%s", buf);        
	}      
        ////               

        player->Position.yPos=nexty; player->Position.xPos=nextx;  
        mvprintw(player->Position.yPos, player->Position.xPos, "%s", ch); 
        move(player->Position.yPos, player->Position.xPos);
 
        player->mode=YES; // AFTER FIRST RUN KEEP PRINTING  

	return player;
}
