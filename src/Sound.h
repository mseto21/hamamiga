#pragma once

#include "Types.h"
#include <SDL_mixer.h>
#include <cstring>
#include <iostream>

struct Sound {
	Mix_Chunk* chunk;
	String128 name;
};

bool Sound_LoadSound(Sound* sound, const char* path, const char* name) {
	sound->chunk = Mix_LoadWAV(path);
	if (!sound->chunk) {
		std::cerr << "Unable to load music from path " << path << ". Error: " << Mix_GetError() << std::endl;
		return false;
	}
	strcpy(&sound->name, path, sizeof(sound->name));
	return true;
}

bool Sound_Play(Sound* sound) {
	if (Mix_PlayChannel(-1, &sound->chunk, 0) == -1) {
		std::cerr << "Unable to play music: " << name << ". Error: " << Mix_GetError() << std::endl;
		return false;
	}
	return true;
}

void Sound_Free(Sound* sound) {
	Mix_FreeChunk(sound->chunk;);
}