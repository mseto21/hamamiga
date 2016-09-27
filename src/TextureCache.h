#include "Texture.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

const int MaxTextures_ = 100;

/* Texture Cache holding all available textures that are currently neeeded. */
struct TextureCache {
	Texture textures[MaxTextures_];
	int index; /* keep track of current asset index */
};

struct TextureCache* tcache; // Global texture cache that works as a singleton


/* Returns the game cache. */
TextureCache* TextureCache_GetCache() {
	if (tcache == nullptr) {
		tcache = (TextureCache*)malloc(sizeof(TextureCache));
		tcache->index = 0;
	}
	return tcache;
}


/* Returns the texture with the specified path. */
Texture* TextureCache_GetTexture(const char* path) {
	// TO-DO: Make this a map, for now we'll do a linear search
	for (int i = 0; i < tcache->index; i++) {
		Texture* texture = &tcache->textures[tcache->index];
		// Since we're comparing const char*, we can compare the pointers.
		if (strcmp(path, texture->path) == 0) {
			return texture;
		}
	}
	return NULL;
}


/* Returns a texture at a particular index. */
Texture* TextureCache_GetTexture(int index) {
	return &tcache->textures[index];
}


/* Creates and returns a new texture from the given path. */
Texture* TextureCache_CreateTexture(const char* path, SDL_Renderer* renderer) {
	Texture_LoadTexture(&tcache->textures[tcache->index], path, renderer);
	return &tcache->textures[tcache->index++];
}


/* Gets or Creates a texture depending on availability. */
Texture* TextureCache_GetOrCreateTexture(const char* path, SDL_Renderer* renderer) {
	Texture* texture;
	if ((texture = TextureCache_GetTexture(path)) == NULL) {
		texture = TextureCache_CreateTexture(path, renderer);
	}
	return texture;
}


/* Frees all of the textures and the cache. */
void TextureCache_Free() {
	for (int i = 0; i < tcache->index; i++) {
		Texture_Free(&tcache->textures[i]);
	}
	free(tcache);
}