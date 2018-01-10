#include "global.h"

int Combat(Level *leveli, Level level, Entity *player, Entity *monster, int attacker)
{
	char buf[8];
	
	if(attacker==PLAYER)
	{
		move(player->Position.yPos,player->Position.xPos);
		// ATTACK BACK:
		if(monster->health>0)
		{
			monster->health -= player->attack;
			sprintf(buf,"%d",player->attack);
			mvprintw(0,0,"\n");
			mvprintw(0,0,"You hit the ");
			printw(monster->name); printw(" for "); printw(buf); printw(" damage!"); 
			refresh(); getch();	

			mvprintw(0,0,"\n"); refresh();

			// If the monster is following, don't fight back!
			if(monster->type!=FOLLOW)
			{
			player->health -= monster->attack;
			sprintf(buf,"%d",monster->attack);
			mvprintw(0,0,"\n");
			mvprintw(0,0,"You were hit by a ");
			printw(monster->name); printw(" for "); printw(buf); printw(" damage!"); 
			refresh(); getch();	
			mvprintw(0,0,"\n"); refresh();
			}
		} else
		{
			if(monster->alive==DEAD)
			{
				Eat(leveli,level,player,monster);
			} else
			{
			monster->alive=DEAD;
			sprintf(buf,"%d",monster->attack);
			mvprintw(0,0,"\n");
			mvprintw(0,0,"Congratulations you killed a ");
			printw(monster->name); printw("!  You gained "); printw(buf); printw(" XP points!");
			player->xp += monster->attack;
			refresh(); getch();
			}
		}
	}
	
	if(attacker==MONSTER)
	{
		if(monster->health>0)
		{
			player->health -= monster->attack;
			sprintf(buf,"%d",monster->attack);
			mvprintw(0,0,"\n");
			mvprintw(0,0,"You were hit by a ");
			printw(monster->name); printw(" for "); printw(buf); printw(" damage!"); 
			refresh(); getch();	
			mvprintw(0,0,"\n"); refresh();
		} else
		{	// ELSE THE MONSTER CANNOT ATTACK!
			if(monster->alive==DEAD)
			{
//				Eat(leveli,level,player,monster);
			} else
			{
			monster->alive=DEAD;
			sprintf(buf,"%d",monster->attack);
			mvprintw(0,0,"\n");
			mvprintw(0,0,"Congratulations you killed a ");
			printw(monster->name); printw("!  You gained "); printw(buf); printw(" XP points!");
			player->xp += monster->attack;
			refresh(); getch();
			}
			monster->alive=DEAD;
		}
	}

	if(player->health<0)
	{
		player->alive=DEAD;
		leveli->killer = monster->name;
	}

	return 1;
}
/*
int KillEntity(Entity *entity, Level level)
{
	PrintChar(entity->Position.yPos,entity->Position.xPos,level.map_backup[entity->Position.yPos][entity->Position.xPos]); 

	return 1;
}
*/
int Eat(Level *leveli, Level level, Entity *player, Entity *monster)
{
//	char buf[8];

	mvprintw(0,0,"\n");	
	mvprintw(0,0,"You see a corpse in front of you, do you want to eat it? y/n");
	refresh(); mvprintw(0,0,"\n");	
	player->input = getchar();
	if(player->input=='y' || player->input=='Y')
	{
		mvprintw(0,0,"You ate the corpse!");
		refresh(); getchar(); mvprintw(0,0,"\n");	
		
		mvprintw(0,0,"You turned into a ");
//		sprintf(buf,"%c",monster->symbol);		
		printw(monster->name); printw("!");
		// TURN INTO:
		player->name = monster->name;
		PrintChar(player->Position.yPos,player->Position.xPos,level.map_backup[player->Position.yPos][player->Position.xPos]);
		PrintChar(monster->Position.yPos,monster->Position.xPos,monster->symbol);
		// CHANGE POSITIONS:	
		player->Position.yPos = monster->Position.yPos;
		player->Position.xPos = monster->Position.xPos;
		move(player->Position.yPos,player->Position.xPos);
		
		// DO OTHER MANIPULATIONS:	
		leveli->player_symbol = monster->symbol;
//		player->symbol = leveli->player_symbol;
		player->health = ((monster->maxhp)*4)+1;
		player->attack = monster->attack;

		refresh(); getchar(); mvprintw(0,0,"\n");	
	
	} else
	{
		move(player->Position.yPos,player->Position.xPos);
	}
	return 1;
}
 
