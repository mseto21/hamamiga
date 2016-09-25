#include "Texture.h"
#include <cstdlib>

const int MaxTextures_ = 100;

struct TextureCache {
	Texture textures[MaxTextures_];
	int index; /* keep track of current asset index */
};

struct TextureCache* tcache; // Global texture cache that works as a singleton

TextureCache* TextureCache_GetCache() {
	if (tcache == nullptr) {
		tcache = (TextureCache*)malloc(sizeof(TextureCache));
		tcache->index = 0;
	}
	return tcache;
}

Texture* TextureCache_GetTexture(const char* path) {
	// TO-DO: Make this a map, for now we'll do a linear search
	for (int i = 0; i < tcache->index; i++) {
		Texture* texture = &tcache->textures[tcache->index];
		// Since we're comparing const char*, we can compare the pointers.
		if (path == texture->path) {
			return texture;
		}
	}
	return NULL;
}

Texture* TextureCache_GetTexture(int index) {
	return &tcache->textures[index];
}

Texture* TextureCache_CreateTexture(const char* path, SDL_Renderer* renderer) {
	Texture_LoadTexture(&tcache->textures[tcache->index], path, renderer);
	return &tcache->textures[tcache->index++];
}

Texture* TextureCache_GetOrCreateTexture(const char* path, SDL_Renderer* renderer) {
	Texture* texture;
	if ((texture = TextureCache_GetTexture(path)) == NULL) {
		texture = TextureCache_CreateTexture(path, renderer);
	}
	return texture;
}

void TextureCache_Free() {
	for (int i = 0; i < tcache->index; i++) {
		Texture_Free(&tcache->textures[i]);
	}
	free(tcache);
}