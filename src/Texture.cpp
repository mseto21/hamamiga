#include "Texture.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

bool Texture_LoadTexture(Texture* texture, const char* path, SDL_Renderer* renderer) {
	if (!texture) {
		std::cerr << "Error: Uninitialized texture!" << std::endl;
		return false;
	}

	SDL_Texture* nTexture = texture->sdltexture;
	SDL_Surface* surface;

	if ((surface = IMG_Load(path)) == NULL) {
		std::cerr << "Could not load " << path << "! Error: " << IMG_GetError() << std::endl;
		return false;
	}

	nTexture = SDL_CreateTextureFromSurface(renderer, surface);

	if (nTexture == NULL) {
		std::cerr << "Unable to create texture from " << path << "! Error:" << IMG_GetError() << std::endl;
		return false;
	}

	texture->w = surface->w;
	texture->h = surface->h;
	texture->path = path;

	SDL_FreeSurface(surface);

	return true;
}

bool Texture_CreateBlank(Texture* texture, SDL_Renderer* renderer) {
	// TO-DO: We may want a blank texture.
	(void) texture;
	(void) renderer;
	return true;
}

void Texture_Free(Texture* texture) {
	if (texture) {
		SDL_DestroyTexture(texture->sdltexture);
		texture = NULL;
	}
}