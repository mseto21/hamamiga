#pragma once

struct _Mix_Music;
struct ComponentBag;

struct SoundSystem {
	_Mix_Music * music;
};

void SoundSystem_Initialize(SoundSystem* soundSystem, ComponentBag* cBag, _Mix_Music* music);
void SoundSystem_Update(SoundSystem* soundSystem, int elapsed);