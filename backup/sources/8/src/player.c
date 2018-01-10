#include "global.h"

Entity *PlayerSetup(Entity *player, Entity room)
{
	player = malloc(sizeof(Entity));

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
	player->health = 20;

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
                mvprintw(player->Position.yPos,player->Position.xPos, buf);        
	}      
        ////               

        player->Position.yPos=nexty; player->Position.xPos=nextx;  
        mvprintw(player->Position.yPos, player->Position.xPos, ch); 
        move(player->Position.yPos, player->Position.xPos);
 
        player->mode=YES; // AFTER FIRST RUN KEEP PRINTING  

	return player;
}

Entity *MonsterMove(Entity *monster, char *buf, char c, int nexty, int nextx)
{
	char ch[8];
	sprintf(ch,"%c",c);
        ///// FOR FIRST MOVEMENT BUG FIXING!!://////  
        if(monster->mode==YES) // As long as the there was one movement:
        { // cause for the bug:   
                mvprintw(monster->Position.yPos,monster->Position.xPos, buf);        
	}      
        ////               

        monster->Position.yPos=nexty; monster->Position.xPos=nextx;  
        mvprintw(monster->Position.yPos, monster->Position.xPos, ch); 
 
        monster->mode=YES; // AFTER FIRST RUN KEEP PRINTING  

	return monster;
}

