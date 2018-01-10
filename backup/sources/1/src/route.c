#include "rogue.h"

int RouteRooms(Position *doors1, Position *doors2, int i, int j)
{
	// Old positions
	int x1 = doors1[i].xPos; int y1 = doors1[i].yPos;
	int x2 = doors2[j].xPos; int y2 = doors2[j].yPos;
	int prex1=x1; int prey1=y1; 
	int prex2=x2; int prey2=y2;

	while(1) 
	{
			
		if(abs((x1-1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1-1))==' '))
		{ // Distance between step is smaller AND there is free space
			prex1 = x1; prex2 = x2;
			x1 = x1-1; 
		} else if(abs((x1-1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1-1))=='.'))
		{ // If distance smaller AND there is an open space
			prex1 = x1; prex2 = x2;
			x1 = x1-1;
		} else if(abs((x1+1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1+1))==' '))
		{
			prex1 = x1; prex2 = x2;
			x1 = x1+1;
		} else if(abs((x1+1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1+1))=='.'))
		{
			prex1 = x1; prex2 = x2;
			x1 = x1+1; 
		} else if(abs((y1-1)-y2)<abs(y1-y2)&&(mvinch((y1-1),x1)==' '))
		{
			prey1 = y1; prey2 = y2;
			y1 = y1-1;
		} else if(abs((y1-1)-y2)<abs(y1-y2)&&(mvinch((y1-1),x1)=='.'))
		{
			prey1 = y1; prey2 = y2;
			y1 = y1-1;
		} else if(abs((y1+1)-y2)<abs(y1-y2)&&(mvinch((y1+1),x1)==' '))
		{
			prey1 = y1; prey2 = y2;
			y1 = y1+1; 
		} else if(abs((y1+1)-y2)<abs(y1-y2)&&(mvinch((y1+1),x1)=='.'))
		{
			prey1 = y1; prey2 = y2;
			y1 = y1+1; 
		} else 
		{
			return 0; 
		}
		mvprintw(y1,x1,"."); // Print free space
	}	
	return 1;
}

int ConnectRooms(Position *doors1, Position *doors2, int d)
{
	int dist = 9999; int I=0; int J=0;
	// Check which doors are the closest to each other:
	for(int i = 0; i<d; i++)
	{
		for(int j = 0; j<d; j++)
		{	// If previous distance is bigger than new distance:
			if(dist > (abs(doors1[i].xPos-doors2[j].xPos)+abs(doors1[i].yPos-doors2[j].yPos)))
			{
				I = i; J = j; // Save the marking numbers 	
				dist = abs(doors1[i].xPos-doors2[j].xPos)+abs(doors1[i].yPos-doors2[j].yPos);
			} else dist=dist;
		}
	}

	RouteRooms(doors1,doors2,I,J);

	return 1;
}

