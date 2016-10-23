#pragma once

struct _Mix_Music;
struct ComponentBag;

struct SoundCheck {
	bool walking = false;
	bool collide = false;
}

struct SoundSystem {
	_Mix_Music * music;
	SoundCheck soundCheck;
};

void SoundSystem_Initialize(SoundSystem* soundSystem, ComponentBag* cBag, _Mix_Music* music);
void SoundSystem_Update(SoundSystem* soundSystem, int elapsed);