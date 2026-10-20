#include "global.h"
#include "intro.h"

int IntroScreen()
{
	clear();
	mvprintw(1,0,intro0);
	printw(intro1);
	printw(intro2);
	
	printw("\n\n\n\n\n                                                                       PRESS A KEY TO CONTINUE\n");

	getch();
	getchar();

	return 1;
}

int WelcomeTxt()
{
	// WELCOME TEXT + INSTRUCTIONS:
	mvprintw(0,0,"\n"); // Clear the line	
	mvprintw(0,0, "Welcome! Press 'Q' to exit and move with wasd+qezx.  You gain XP from killing monsters and going deeper!");
	refresh(); getchar();
	mvprintw(0,0,"\n"); // Clear the line	

	return 1;
}

int PlayerInfoBar(Level *level)
{
	char *hunger_status = "Normal";
	int h = level[level->lvl].player->hunger;
	if(h >= 750)
	{
		hunger_status = "Satiated";
	} else if(h >= 350)
	{
		hunger_status = "Normal";
	} else if(h >= 150)
	{
		hunger_status = "Hungry";
	} else if(h > 0)
	{
		hunger_status = "Weak";
	} else
	{
		hunger_status = "Starving";
	}

	mvprintw(MAX_Y, 0,"    Player: "); printw(level[level->lvl].player->name);
	printw("    Hp: %d/%d", level[level->lvl].player->health, level[level->lvl].player->maxhp);
	printw("    Att: %d", level[level->lvl].player->attack);
	printw("    Level: %d", level->lvl);
	printw("    Hunger: %s", hunger_status);
	if(level[level->lvl].player->poison_turns > 0)
	{
		printw(" [POISON:%d]", level[level->lvl].player->poison_turns);
	}
	printw("    XP: %d", level[level->lvl].player->xp);
	printw("      ");

	return 1;
}

int ExitScreen(Level *level)
{
	char buf[8];
	sprintf(buf,"%d",level[level->lvl].player->xp);
	clear();

	mvprintw(25,0,"                                                               YOU WERE A "); printw(level[level->lvl].player->name); 
	printw(" KILLED BY A "); printw(level->killer); 
	printw(" AND DIED WITH "); printw(buf); printw(" XP!");
	printw("\n\n\n\n\n                                                                                   GAME OVER!");
	refresh(); getchar();
	
	clear();
	mvprintw(27,76,"THANK YOU FOR PLAYING!");
	refresh(); getchar();

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
	if(player->can_open_doors == NO)
	{
		mvprintw(0,0,"\n");
		mvprintw(0,0,"You cannot open doors as a %s! (You lack hands)", player->name);
		refresh(); getch();
		mvprintw(0,0,"\n");
		refresh();
		move(player->Position.yPos, player->Position.xPos);
		return 0;
	}

	mvprintw(0,0,"\n"); // Clear the line	
	mvprintw(0,0, "Do you want to open the door? y/n");
	refresh(); mvprintw(0,0,"\n");
	player->input = getchar();
	if(player->input=='y' || player->input=='Y')
	{
	
		mvprintw(0,0, "You opened the door! (close a door with 'c')");
		refresh(); getchar(); mvprintw(0,0,"\n");
		PrintChar(y,x,':');
//		mvprintw(y,x,":");
	} 
	else
	{
		move(player->Position.yPos,player->Position.xPos);
	}
	return 1;
}

