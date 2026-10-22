// TO AVOID REUSING THIS CODE TO MAKE NEW VARIABLES EACH TIME:
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Function names
Level MakeLvl(Level level_in, int lvl, Level *leveli);
Level *Game(Level *level);
Level *Downstairs(Level *level, int r);
Level *Upstairs(Level *level, int r);

Entity MapInit(Entity room); 

Entity *CheckNextChar(Level *leveli, char c, int type, int y, int x, Level level, Entity *monster);

Entity *PlayerSetup(Level *leveli, Entity *player, Entity room); 
Entity *PlayerMove(Entity *player, char *buf, char c, int nexty, int nextx);
void ApplyFormTraits(Entity *entity, char symbol);
void CopyPlayer(Entity *dest, Entity *src);
Entity *CreateMon(char c, int hp, int att, int mode, int type, char *name);
Entity *SelMonster(int lvl);
Entity *GetMonsterAt(int MonY, int MonX, Level level);
Entity *MonsterMove(Level level, Entity *monster, char c1, char c2, int nexty, int nextx, int mode);
Entity **AddMonsters(Level *level, Entity room, int lvl);

Position CheckArea(Position pos, char c); 
Position CreateRoomSize(Position pos); 
Position DrawDoor(int height, int width, Position pos, int i); 
Position *DrawRoom(Position pos, int d); 

int Eat(Level *leveli, Level level, Entity *player, Entity *monster);
int DropCorpse(Level *leveli, Entity *monster, int y, int x);
Corpse *GetCorpseAt(Level level, int y, int x);
const char *GetCorpseStage(int age);
int AskEatCorpse(Level *leveli, Level level, Entity *player, int y, int x);
int EatCorpse(Level *leveli, Level level, Entity *player, Corpse *corpse);
int ActivateFormAbility(Level *leveli, Level level, Entity *player);

int GetPlayerAttack(Entity *player);
int GetPlayerDefense(Entity *player);
void SpawnFloorItems(Level *level, Entity rooms, int lvl);
Item *GetItemAt(Level level, int y, int x);
int PickUpFloorItem(Level *leveli, Level level, Entity *player);
int ShowInventory(Level *leveli, Level level, Entity *player);
int LookMode(Level *leveli, Level level, Entity *player);
int QuickWield(Level *leveli, Level level, Entity *player);
int QuickDrop(Level *leveli, Level level, Entity *player);
int EatFromInventory(Level *leveli, Level level, Entity *player);

int ScreenInit();
int IntroScreen();
int WelcomeTxt();
int PlayerInfoBar(Level *level);
int ExitScreen(Level *level);
int Combat(Level *leveli, Level level, Entity *player, Entity *monster, int attacker);
int KillEntity(Entity *entity, Level level);
int Free(Level level, int lvl, int lvls);
int FreeAll(Level *level, int lvl, int lvls);
int AskStairs(int stairs, Entity *player);
int PrintMap(int y, int x, char **map);
int Route(Position *p1, Position *p2, int i, int j, char chk1, char chk2, char in, int mode);
int ConnectRooms(Position *doors1, Position *doors2, int d);
int TurnNearby(int y, int x, int c, char *d, Level level); 
void ClearMsg(void);
int OpenDoor(Level *leveli, Entity *player, int y, int x);
int PrintChar(int y, int x, char c); 
int PrintNum(int num, char *c); 
int StepOn(Level *leveli, int type, int nexty, int nextx, char c, char symbol, Level level, Entity *monster);

int *PathFindRandom(int *yx, int y, int x, char **map, int mode);
int *PathFindFollow(int *yx, int y1, int x1, int y2, int x2, char **map, int can_phase);
void UpdateFOV(Level *level);
void RenderFOV(Level *level);
int SpawnFogMonster(Level *level);
int *MonSetup(Entity *monster, Entity room, int *yx);
int HandleInput(Level *leveli, Level level, int *yx);
int *RandomCoords(int *yx, int rm, Position room);

char **SaveMap(char **XY, int y, int x);
char **UpdateMap(char **map, int y, int x);

#endif // TO END THE DEFINITION FUNCTION TO ONE TIME USE ONLY!
