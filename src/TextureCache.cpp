#include "TextureCache.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <SDL.h>

struct TextureCache* tcache; // Global texture cache that works as a singleton

/* Returns the texture cache. */
TextureCache* TextureCache_GetCache() {
	if (tcache == nullptr) {
		tcache = (TextureCache*)malloc(sizeof(TextureCache));
		tcache->index = 0;
		tcache->levelIndex = 0;
	}
	return tcache;
}


/* Creates and returns a new texture from the given path. */
Texture* TextureCache_CreateTexture(SDL_Renderer* renderer, const char* path, const char* name) {
	// Check if we already have that texture.
	int textureIndex = 0;
	for (; textureIndex < tcache->index; textureIndex++) {
		if (strcmp(tcache->textures[textureIndex].name, name) == 0) {
			TextureCache_Remove(name);
			break;
		}
	}

	// Load texture if not loaded.
	Texture_LoadTexture(&tcache->textures[textureIndex], renderer, path, name);
	SDL_SetTextureBlendMode(tcache->textures[textureIndex].sdltexture , SDL_BLENDMODE_BLEND );
	if (textureIndex == tcache->index)
		tcache->index++;
	return &tcache->textures[textureIndex];
}


/* Creates and returns a new texture with the given message, color, and font*/
Texture* TextureCache_CreateFont(SDL_Renderer* renderer, _TTF_Font* font, SDL_Color color, const char* message, const char* name) {
	Texture_CreateTextureFromFont(&tcache->textures[tcache->index], renderer, font, color, message, name);
	return &tcache->textures[tcache->index++];
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


/* Gets or Creates a texture depending on availability. */
Texture* TextureCache_GetOrCreateTexture(SDL_Renderer* renderer, const char* path, const char* name) {
	Texture* texture;
	if ((texture = TextureCache_GetTexture(name)) == NULL) {
		texture = TextureCache_CreateTexture(renderer, path, name);
	}
	return texture;
}


/* Replaces the first texture with the given string. */
Texture* TextureCache_ReplaceTexture(SDL_Renderer* renderer, const String128 path, const String128 name) {
	(void) renderer;
	int textureIndex = 0;
	for (; textureIndex < tcache->index; textureIndex++) {
		if (strcmp(tcache->textures[textureIndex].name, name) == 0) {
			break;
		}
	}
	if (textureIndex == tcache->index) {
		TextureCache_CreateTexture(renderer, path, name);
	}
	Texture* old = TextureCache_GetTexture(textureIndex);
	memcpy(old, TextureCache_CreateTexture(renderer, path, name), sizeof(Texture));
	return old;
}


/* Removes the texture with path from the cache. */
void TextureCache_Remove(const char* path) {
	int textureIndex = 0;
	for (; textureIndex < tcache->index; textureIndex++) {
		if (strcmp(tcache->textures[textureIndex].name, path) == 0) {
			memcpy(&tcache->textures[textureIndex], &tcache->textures[tcache->index - 1], sizeof(Texture));
			tcache->index--;
			return;
		}
	}
}

/* Removes all of the textures from when the level was loaded.*/
void TextureCache_FreeLevel() {
	int index = tcache->levelIndex;
	for (; index < tcache->index; index++) {
		memset(&tcache->textures[index], 0, sizeof(struct Texture));
	}
	tcache->index = tcache->levelIndex;
	tcache->levelIndex = 0;
}


/* Frees all of the textures and the cache. */
void TextureCache_Free() {
	for (int i = 0; i < tcache->index; i++) {
		Texture_Free(&tcache->textures[i]);
	}
	free(tcache);
	tcache = nullptr;
}