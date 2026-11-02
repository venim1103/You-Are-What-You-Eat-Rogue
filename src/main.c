#include "global.h"

// Functions
int main(void) 
{

// CREATE THE LEVEL POINTER
	Level *level=NULL;

// Seed the random (supports Web Seed / Daily Challenge or CLI env seed)
#ifdef __EMSCRIPTEN__
	GAME_SEED = web_get_initial_seed(&GAME_SEED_MODE, GAME_SEED_LABEL, (int)sizeof(GAME_SEED_LABEL));
	if(GAME_SEED == 0)
	{
		GAME_SEED = (unsigned int)time(NULL);
		if(GAME_SEED == 0) GAME_SEED = 1;
	}
	if(GAME_SEED_LABEL[0] == '\0')
	{
		snprintf(GAME_SEED_LABEL, sizeof(GAME_SEED_LABEL), "%u", GAME_SEED);
	}
#else
	const char *env_seed = getenv("ROGUE_SEED");
	if(env_seed != NULL && env_seed[0] != '\0')
	{
		unsigned long parsed = strtoul(env_seed, NULL, 10);
		if(parsed == 0)
		{
			unsigned int h = 2166136261u;
			for(const char *p = env_seed; *p; p++)
			{
				h ^= (unsigned char)(*p);
				h *= 16777619u;
			}
			GAME_SEED = (h != 0) ? h : 1u;
		}
		else
		{
			GAME_SEED = (unsigned int)parsed;
		}
		GAME_SEED_MODE = 2;
		snprintf(GAME_SEED_LABEL, sizeof(GAME_SEED_LABEL), "%.28s", env_seed);
	}
	else
	{
		GAME_SEED = (unsigned int)time(NULL);
		if(GAME_SEED == 0) GAME_SEED = 1;
		GAME_SEED_MODE = 0;
		snprintf(GAME_SEED_LABEL, sizeof(GAME_SEED_LABEL), "%u", GAME_SEED);
	}
#endif
	srand(GAME_SEED);
// Init
	ScreenInit();

// MAIN LOOP
	level = Game(level);

// Exit
	// level[0].maxlvl is the last created lvl id number
	ExitScreen(level);

	FreeAll(level, level->lvl, level->maxlvl);

	endwin();
	return 0;
}

