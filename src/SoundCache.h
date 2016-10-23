#include "Types.h"
#include "Sound.h"

const int MaxSounds_ = 100;

struct SoundCache {
	Sound sounds[MaxSounds_];
	int index;
};


struct SoundCache* scache; // Global texture cache that works as a singleton
SoundCache* SoundCache_GetCache();
Mix_Music* SoundCache_CreateSound(const char* path, const char* name);
Mix_Music* SoundCache_GetSound(const char* path);
void SoundCache_Free();