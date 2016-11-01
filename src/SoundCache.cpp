#include "SoundCache.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

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
Sound* SoundCache_CreateSound(const char* path, const char* name) {
	// Check if we already have the sound
	for (int soundIndex = 0; soundIndex < scache->index; soundIndex++) {
		if (strcmp(scache->sounds[soundIndex].name, name) == 0) {
			return &scache->sounds[soundIndex];
		}
	}

	// Load sound if not loaded.
	Sound_LoadSound(&scache->sounds[scache->index], path, name);
	return &scache->sounds[scache->index++];
}


/* Returns the sound if present. */
Sound* SoundCache_GetSound(const char* path) {
	for (int i = 0; i < scache->index; i++) {
		Sound* sound = &scache->sounds[i];
		if (strcmp(path, sound->name) == 0) {
			return sound;
		}
	}
	return NULL;
}

/* Frees the sound if present. */
void SoundCache_FreeSound(const char* path) {
	for (int i = 0; i < scache->index; i++) {
		Sound* sound = &scache->sounds[i];
		if (strcmp(path, sound->name) == 0) {
			Sound_Free(&scache->sounds[i]);
			return;
		}
	}
}


/* Frees the sound cache. */
void SoundCache_Free() {
	for (int soundIndex = 0; soundIndex < scache->index; soundIndex++) {
		Sound_Free(&scache->sounds[soundIndex]);
	}
	free(scache);
	scache = nullptr;
}