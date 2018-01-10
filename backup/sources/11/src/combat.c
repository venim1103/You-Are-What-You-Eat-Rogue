#include "global.h"

int Combat(Entity *player, Entity *monster, int attacker)
{
	if(attacker==PLAYER)
	{
		monster->health -= player->attack;
		PrintNum(player->attack,"You hit the enemy for:   !");
		mvprintw(0,0,"\n"); refresh();
		getch();	
		move(player->Position.yPos,player->Position.xPos);
		// ATTACK BACK:
		if(monster->health>0)
		{	// If the monster is following, don't fight back!
			if(monster->type!=FOLLOW)
			{
			player->health -= monster->attack;
			PrintNum(monster->attack,"You were hit for:   !");
			mvprintw(0,0,"\n"); refresh();
			}
		} else
		{
			monster->alive=DEAD;
		}
	}
	
	if(attacker==MONSTER)
	{
		if(monster->health>0)
		{
			player->health -= monster->attack;
			PrintNum(monster->attack,"You were hit for:   !");
			mvprintw(0,0,"\n"); refresh();
		} else
		{
			monster->alive=DEAD;
		}
	}
	return 1;
}

int KillEntity(Entity *entity, Level level)
{
	PrintChar(entity->Position.yPos,entity->Position.xPos,level.map_backup[entity->Position.yPos][entity->Position.xPos]); 

	return 1;
}
