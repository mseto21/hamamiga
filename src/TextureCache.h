#include "Texture.h"
#include "Types.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <SDL.h>

const int MaxTextures_ = 100;

/* Texture Cache holding all available textures that are currently neeeded. */
struct TextureCache {
	Texture textures[MaxTextures_];
	int index; /* keep track of current asset index */
};

struct TextureCache* tcache; // Global texture cache that works as a singleton


/* Returns the texture cache. */
TextureCache* TextureCache_GetCache() {
	if (tcache == nullptr) {
		tcache = (TextureCache*)malloc(sizeof(TextureCache));
		tcache->index = 0;
	}
	return tcache;
}


/* Returns the texture with the specified path. */
Texture* TextureCache_GetTexture(const String128 path) {
	for (int i = 0; i < tcache->index; i++) {
		Texture* texture = &tcache->textures[i];
		if (strcmp(path, texture->name) == 0) {
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
Texture* TextureCache_CreateTexture(SDL_Renderer* renderer, const char* path, const char* name) {
	Texture_LoadTexture(&tcache->textures[tcache->index], renderer, path, name);
	SDL_SetTextureBlendMode(tcache->textures[tcache->index].sdltexture , SDL_BLENDMODE_BLEND );
	return &tcache->textures[tcache->index++];
}


/* Creates and returns a new texture with the given message, color, and font*/
Texture* TextureCache_CreateFont(SDL_Renderer* renderer, _TTF_Font* font, SDL_Color color, const char* message, const char* name) {
	Texture_CreateTextureFromFont(&tcache->textures[tcache->index], renderer, font, color, message, name);
	return &tcache->textures[tcache->index++];
}


/* Gets or Creates a texture depending on availability. */
Texture* TextureCache_GetOrCreateTexture(SDL_Renderer* renderer, const char* path, const char* name) {
	Texture* texture;
	if ((texture = TextureCache_GetTexture(name)) == NULL) {
		texture = TextureCache_CreateTexture(renderer, path, name);
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