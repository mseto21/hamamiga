#include "Types.h"
#include "Sound.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

const int MaxSounds_ = 100;

struct SoundCache {
	Sound sounds[MaxSounds_];
	int index;
};


struct SoundCache* scache; // Global texture cache that works as a singleton


/* Returns the texture cache. */
SoundCache* SoundCache_GetCache() {
	if (scache == nullptr) {
		scache = (SoundCache*)malloc(sizeof(SoundCache));
		scache->index = 0;
	}
	return scache;
}


/* Returns a newly created sound. */
Mix_Music* SoundCache_CreateSound(const char* path, const char* name) {
	return &Sound_Load(&scache->sounds[scache->index], path, name);
}


/* Returns the sound if present. */
MixMusic* SoundCache_GetSound(const char* path) {
	for (int i = 0; i < scache->index; i++) {
		Sound* sound = &scache->sounds[i];
		if (strcmp(path, sound->name) == 0) {
			return sound;
		}
	}
	return NULL;
}


/* Frees the sound cache. */
void SoundCache_Free() {
	for (int soundIndex = 0; soundIndex < scache->index) {
		Sound_Free(&scache->sounds[soundIndex]);
	}
	free scache;
}