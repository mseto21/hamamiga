#include "Sound.h"
#include "Constants.h"
#include <cstring>
#include <iostream>

bool Sound_LoadSound(Sound* sound, const char* path, const char* name) {
	sound->chunk = Mix_LoadWAV(path);
	strcpy(sound->name, name);
	if (!sound->chunk) {
		std::cerr << "Unable to load music from path " << path << ". Error: " << Mix_GetError() << std::endl;
		return false;
	}
	return true;
}

bool Sound_Play(Sound* sound, int loops) {
	if (strcmp(sound->name, "disco") == 0){
		if (Mix_PlayChannel(Constants::DiscoChannel_, sound->chunk, loops) == -1) {
			std::cerr << "Unable to play music: " << sound->name << ". Error: " << Mix_GetError() << std::endl;
			return false;
		}
	} else if (Mix_PlayChannel(Constants::SoundChannel_, sound->chunk, loops) == -1) {
		std::cerr << "Unable to play music: " << sound->name << ". Error: " << Mix_GetError() << std::endl;
		return false;
	}
	return true;
}

void Sound_Free(Sound* sound) {
	if (sound->chunk)
		Mix_FreeChunk(sound->chunk);
}