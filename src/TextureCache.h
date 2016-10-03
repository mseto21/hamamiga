#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

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

//struct TextureCache* tcache; // Global texture cache that works as a singleton

TextureCache* TextureCache_GetCache();
Texture* TextureCache_GetTexture(const char* path);
Texture* TextureCache_GetTexture(int index);
Texture* TextureCache_CreateTexture(const char* path, SDL_Renderer* renderer);
Texture* TextureCache_GetOrCreateTexture(const char* path, SDL_Renderer* renderer);
void TextureCache_Free();


#endif