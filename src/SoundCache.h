#pragma once
#include "Types.h"
#include "Sound.h"

const int MaxSounds_ = 100;

struct SoundCache {
	Sound sounds[MaxSounds_];
	int index;
};

SoundCache* SoundCache_GetCache();
Sound* SoundCache_CreateSound(const char* path, const char* name);
Sound* SoundCache_GetSound(const char* path);
void SoundCache_FreeSound(const char* path);
void SoundCache_Free();