#include "Sound.h"
#include <cstring>
#include <iostream>

bool Sound_LoadSound(Sound* sound, const char* path, const char* name) {
	sound->chunk = Mix_LoadWAV(path);
	if (!sound->chunk) {
		std::cerr << "Unable to load music from path " << path << ". Error: " << Mix_GetError() << std::endl;
		return false;
	}
	strcpy(sound->name, name);
	return true;
}

bool Sound_Play(Sound* sound, int loops) {
	if (Mix_PlayChannel(2, sound->chunk, loops) == -1) {
		std::cerr << "Unable to play music: " << sound->name << ". Error: " << Mix_GetError() << std::endl;
		return false;
	}
	return true;
}

void Sound_Free(Sound* sound) {
	Mix_FreeChunk(sound->chunk);
}