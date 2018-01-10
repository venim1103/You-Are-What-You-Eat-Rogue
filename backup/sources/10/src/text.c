#include "global.h"

int WelcomeTxt()
{
	// WELCOME TEXT + INSTRUCTIONS:
	mvprintw(0,0,"\n"); // Clear the line	
	mvprintw(0,0, "Welcome! Press 'Q' to exit and move with wasd+qezx");
	refresh(); getchar();
	mvprintw(0,0,"\n"); // Clear the line	

	return 1;
}

int PlayerInfoBar(Level *level)
{
	mvprintw(MAX_Y, 0, "    Level: %d", level->lvl);
	printw("    Hp: %d", level[level->lvl].player->health);
	printw("    Att: %d", level[level->lvl].player->attack);
	printw("    XP: %d", level[level->lvl].player->xp);
	printw("      ");

	return 1;
}

int PrintChar(int y, int x, char c)
{
	char buf[8]; 
	sprintf(buf, "%c", c);
	mvprintw(y,x,buf);
	return 1;
}

int PrintNum(int num, char *c)
{
	char buf[8];
	
	int len = 1; len = strlen(c);
	
	mvprintw(0,0,"\n");	
	mvprintw(0,0,c);
	sprintf(buf,"%d",num);	
	mvprintw(0,(len-3),buf);	
	refresh();
	getch();
	
	return 1;
}

int PrintMap(int y, int x, char **map)
{
	clear();
	for(int i=0; i<y; i++)
	{
		for(int j=0; j<x; j++)
		{
			PrintChar(i,j,map[i][j]);
		}
	}
	return 1;
}

int AskStairs(int stairs, Entity *player)
{
	switch(stairs)
	{
	case DSTAIRS:
		mvprintw(0,0,"\n"); // Clear the line	
		mvprintw(0,0,"You see downstairs here, would you like to enter? y/n");	
		refresh(); mvprintw(0,0,"\n");
		player->input = getchar();

		if(player->input=='y' || player->input=='Y')
		{
			player->Position.type=DSTAIRS;		
		} 
	break;
	
	case USTAIRS:
		mvprintw(0,0,"\n"); // Clear the line	
		mvprintw(0,0,"You see upstairs here, would you like to enter? y/n");	
		refresh(); mvprintw(0,0,"\n");
		player->input = getchar();
		if(player->input=='y' || player->input=='Y')
		{
			player->Position.type=USTAIRS;		
		} 
	break;

	default:
		mvprintw(0,0,"\n"); // Clear the line	
		mvprintw(0,0,"ERROR!!!!");	
		refresh(); mvprintw(0,0,"\n");
	break;
	}

	return 1;
}

int OpenDoor(Entity *player, int y, int x)
{
	mvprintw(0,0,"\n"); // Clear the line	
	mvprintw(0,0, "Do you want to open the door? y/n");
	refresh(); mvprintw(0,0,"\n");
	player->input = getchar();
	if(player->input=='y' || player->input=='Y')
	{
	
		mvprintw(0,0, "You opened the door! (close a door with 'c')");
		refresh(); getchar(); mvprintw(0,0,"\n");
		mvprintw(y,x,":");
	} 
	else
	{
		move(player->Position.yPos,player->Position.xPos);
	}
	return 1;
}

