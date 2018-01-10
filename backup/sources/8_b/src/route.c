#include "global.h"

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

	Route(doors1,doors2,I,J, ' ', '#', '#', YES);

	return 1;
}

int Route(Position *p1, Position *p2, int i, int j, char chk1, char chk2, char in, int mode)
{
	int x1,x2,y1,y2 = 0;
	x1 = p1[i].xPos; y1 = p1[i].yPos;
	x2 = p2[j].xPos; y2 = p2[j].yPos;

	char buf[8];
	sprintf(buf,"%c",in);

	while(1) 
	{
			
	if(abs((x1-1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1-1))==chk1))
		{ // Distance between step smaller AND there is chk1
			x1 = x1-1; 
	} else if(abs((x1-1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1-1))==chk2))
		{ // If distance smaller AND there is chk2
			x1 = x1-1;
	} else if(abs((x1+1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1+1))==chk1))
		{
			x1 = x1+1;
	} else if(abs((x1+1)-x2)<abs(x1-x2)&&(mvinch(y1,(x1+1))==chk2))
		{
			x1 = x1+1; 
	} else if(abs((y1-1)-y2)<abs(y1-y2)&&(mvinch((y1-1),x1)==chk1))
		{
			y1 = y1-1;
	} else if(abs((y1-1)-y2)<abs(y1-y2)&&(mvinch((y1-1),x1)==chk2))
		{
			y1 = y1-1;
	} else if(abs((y1+1)-y2)<abs(y1-y2)&&(mvinch((y1+1),x1)==chk1))
		{
			y1 = y1+1; 
	} else if(abs((y1+1)-y2)<abs(y1-y2)&&(mvinch((y1+1),x1)==chk2))
		{
			y1 = y1+1; 
		} else 
		{
			return 0; 
		}
	
	if(mode==YES)
	{
		mvprintw(y1,x1,buf); // Print
	}
	
	}	
	return 1;
}

