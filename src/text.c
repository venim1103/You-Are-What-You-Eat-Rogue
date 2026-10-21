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

	return 1;
}

void ClearMsg(void)
{
	move(0, 0);
	clrtoeol();
}

int WelcomeTxt()
{
	// WELCOME TEXT + INSTRUCTIONS:
	ClearMsg();
	mvprintw(0,0, "Move: wasd/qezx/numpad | Ability: 'f' | Wait: '.' | Eat: 'e'/'E' | Close: 'c' | Quit: 'Q'");
	refresh(); getch();
	ClearMsg();

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

	move(MAX_Y, 0);
	clrtoeol();
	mvprintw(MAX_Y, 0,"    Player: "); printw(level[level->lvl].player->name);
	printw("    Hp: %d/%d", level[level->lvl].player->health, level[level->lvl].player->maxhp);
	printw("    Att: %d", level[level->lvl].player->attack);
	printw("    Level: %d", level->lvl);
	printw("    Hunger: %s", hunger_status);
	if(level[level->lvl].player->poison_turns > 0)
	{
		printw(" [POISON:%d]", level[level->lvl].player->poison_turns);
	}
	if(level[level->lvl].player->ability_name != NULL)
	{
		if(level[level->lvl].player->ability_cd > 0)
		{
			printw(" [%s:%dt]", level[level->lvl].player->ability_name, level[level->lvl].player->ability_cd);
		}
		else
		{
			printw(" [%s:READY]", level[level->lvl].player->ability_name);
		}
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
	refresh(); getch();
	
	clear();
	mvprintw(27,76,"THANK YOU FOR PLAYING!");
	refresh(); getch();

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
		ClearMsg();
		mvprintw(0,0,"You see downstairs here, would you like to enter? y/n");	
		refresh();
		player->input = getch();
		ClearMsg();

		if(player->input=='y' || player->input=='Y')
		{
			player->Position.type=DSTAIRS;		
		} 
	break;
	
	case USTAIRS:
		ClearMsg();
		mvprintw(0,0,"You see upstairs here, would you like to enter? y/n");	
		refresh();
		player->input = getch();
		ClearMsg();
		if(player->input=='y' || player->input=='Y')
		{
			player->Position.type=USTAIRS;		
		} 
	break;

	default:
		ClearMsg();
		mvprintw(0,0,"ERROR!!!!");	
		refresh();
		ClearMsg();
	break;
	}

	return 1;
}

int OpenDoor(Entity *player, int y, int x)
{
	if(player->can_open_doors == NO)
	{
		ClearMsg();
		mvprintw(0,0,"You cannot open doors as a %s! (You lack hands)", player->name);
		refresh(); getch();
		ClearMsg();
		refresh();
		move(player->Position.yPos, player->Position.xPos);
		return 0;
	}

	ClearMsg();
	mvprintw(0,0, "Do you want to open the door? y/n");
	refresh();
	player->input = getch();
	ClearMsg();
	if(player->input=='y' || player->input=='Y')
	{
		ClearMsg();
		mvprintw(0,0, "You opened the door! (close a door with 'c')");
		refresh(); getch();
		ClearMsg();
		PrintChar(y,x,':');
	} 
	else
	{
		move(player->Position.yPos,player->Position.xPos);
	}
	return 1;
}

