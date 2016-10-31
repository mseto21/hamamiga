#pragma once
#include "Texture.h"
#include "Types.h"

const int MaxTextures_ = 100;

/* Texture Cache holding all available textures that are currently neeeded. */
struct TextureCache {
	Texture textures[MaxTextures_];
	int index; /* keep track of current asset index */
	int levelIndex;
};

TextureCache* TextureCache_GetCache();
Texture* TextureCache_CreateTexture(SDL_Renderer* renderer, const char* path, const char* name);
Texture* TextureCache_CreateFont(SDL_Renderer* renderer, _TTF_Font* font, SDL_Color color, const char* message, const char* name);
Texture* TextureCache_GetTexture(const String128 path);
Texture* TextureCache_GetTexture(int index);
Texture* TextureCache_GetOrCreateTexture(SDL_Renderer* renderer, const char* path, const char* name);
Texture* TextureCache_ReplaceTexture(SDL_Renderer* renderer, const char* path, const char* name);
void TextureCache_Remove(const char* path);
void TextureCache_FreeLevel();
void TextureCache_Free();