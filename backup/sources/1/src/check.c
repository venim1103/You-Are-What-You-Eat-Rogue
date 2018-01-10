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

	char buf[8];
	sprintf(buf,"%c",player->Position.ch); // The character on last position

	switch(c)
	{

	case '+': // If there is a closed door:
		mvprintw(0,0, "Do you want to open the door? y/n");
		refresh(); mvprintw(0,0, "\n");
		player->input = getchar();
		if(player->input=='y'||player->input=='Y') 
		{
			mvprintw(0,0, "You opened the door! (close a door with 'c')");
			refresh(); mvprintw(0,0, "\n"); getchar();
			TurnNearby(player->Position.yPos,player->Position.xPos,'+',":");
		}	
		else // Else just update cursor
		{
			move(player->Position.yPos, player->Position.xPos);
		}	
	break;
	// .. more here such as enemies and loot!

	default: // Print the previous character and only after that update
		mvprintw(player->Position.yPos,player->Position.xPos, buf);
		player->Position.yPos=y; player->Position.xPos=x;
		mvprintw(player->Position.yPos, player->Position.xPos, "@");
		move(player->Position.yPos, player->Position.xPos);
	break;
	}	
	return 1;
}

