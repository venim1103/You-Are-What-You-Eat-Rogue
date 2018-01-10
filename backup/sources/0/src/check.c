#include "rogue.h"

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

