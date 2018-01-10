// TO AVOID REUSING THIS CODE TO MAKE NEW VARIABLES EACH TIME:
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Function names
Level MakeLvl(Level level_in, int lvl, Level *leveli);
Level *Game(Level *level);
Level *Downstairs(Level *level, int r);
Level *Upstairs(Level *level, int r);

Entity MapInit(Entity room); 
Entity *PlayerSetup(Entity *player, Entity room); 
Entity *PlayerMove(Entity *player, char *buf, int nexty, int nextx);
Entity *CreateMon(char c, int hp, int att, int mode);
Entity *SelMonster(int lvl);
Entity **AddMonsters(Level level, Entity room, int lvl, Level *leveli);

Position CheckArea(Position pos, char c); 
Position CreateRoomSize(Position pos); 
Position DrawDoor(int height, int width, Position pos, int i); 
Position *DrawRoom(Position pos, int d); 

int Free(Level level, int lvl, int lvls);
int FreeAll(Level *level, int lvl, int lvls);
int ScreenInit();
int WelcomeTxt();
int AskStairs(int stairs, Entity *player);
int PrintMap(int y, int x, char **map);
int Route(Position *p1, Position *p2, int i, int j, char chk1, char chk2, char in, int mode);
int ConnectRooms(Position *doors1, Position *doors2, int d);
int TurnNearby(int y, int x, int c, char *d); 
int HandleInput(Entity *player, char **map, Entity rooms);
int OpenDoor(Entity *player, int y, int x);
int MonSetup(Entity *monster, Entity room);
int PrintChar(int y, int x, char c); 
int PrintNum(int num, char *c, int size, int pos); 
int StepOn(int y, int x, char c, Entity *player, char **map);
int CheckNextChar(int y, int x, Entity *player, char **map);

int *RandomCoords(int *yx, int rm, Position room);

char **SaveMap(char **XY, int y, int x);
char **UpdateMap(char **map, int y, int x);

#endif // TO END THE DEFINITION FUNCTION TO ONE TIME USE ONLY!
