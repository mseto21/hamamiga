#pragma once

#include "Types.h"
#include <SDL_mixer.h>

struct Sound {
	Mix_Chunk* chunk;
	String128 name;
};

bool Sound_LoadSound(Sound* sound, const char* path, const char* name);
bool Sound_Play(Sound* sound, int loops);
void Sound_Free(Sound* sound);