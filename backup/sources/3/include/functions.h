// TO AVOID REUSING THIS CODE TO MAKE NEW VARIABLES EACH TIME:
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Function names
Level MakeLvl(Level level_in);
Level *Game(Level *level);

Entity MapInit(Entity room); 
Entity *PlayerSetup(Entity *player, Entity room); 

Position CheckArea(Position pos); 
Position CreateRoomSize(Position pos); 
Position DrawDoor(int height, int width, Position pos, int i); 
Position *DrawRoom(Position pos, int d); 

int Free(Level level);
int FreeAll(Level *level, int lvls);
int ScreenInit();
int RouteRooms(Position *doors1, Position *doors2, int i, int j);
int ConnectRooms(Position *doors1, Position *doors2, int d);
int TurnNearby(int y, int x, int c, char *d); 
int HandleInput(Entity *player, char **map, Entity rooms);
int PrintChar(int y, int x, char c); 
int StepOn(int y, int x, char c, Entity *player, char **map);
int CheckNextChar(int y, int x, Entity *player, char **map);
int OpenDoor(Entity *player, int y, int x);

int *RandomCoords(int rm, Position room);

char **SaveMap(char **XY, int y, int x);
char **UpdateMap(char **map, int y, int x);

#endif // TO END THE DEFINITION FUNCTION TO ONE TIME USE ONLY!
