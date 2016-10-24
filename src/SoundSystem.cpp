#include "SoundSystem.h"
#include <SDL_mixer.h>

void SoundSystem_Initialize(SoundSystem* soundSystem, ComponentBag* cBag, _Mix_Music* music) {
	soundSystem->music = music;
}

void SoundSystem_Update(SoundSystem* soundSystem, int elapsed) {
	if (Mix_PlayingMusic() == 0) {
		Mix_PlayMusic(soundSystem->music, -1);
		Mix_VolumeMusic(MIX_MAX_VOLUME);
	}
}