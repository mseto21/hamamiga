#include "Texture.h"
#include "StringOperations.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstring>
#include <iostream>

//--------------------------------------------------------------------
bool Texture_LoadTexture(Texture* texture, SDL_Renderer* renderer, const char* path, const char* name) {
	if (!renderer) {
		std::cerr << "Error: The renderer was null!" << std::endl;
		return false;
	}
	if (!texture) {
		std::cerr << "Error: Uninitialized texture!" << std::endl;
		return false;
	}

	SDL_Surface* surface;

	if ((surface = IMG_Load(path)) == NULL) {
		std::cerr << "Could not load " << path << "! Error: " << IMG_GetError() << std::endl;
		return false;
	}

	texture->sdltexture = SDL_CreateTextureFromSurface(renderer, surface);

	if (texture->sdltexture == NULL) {
		std::cerr << "Unable to create texture from " << path << "! Error:" << IMG_GetError() << std::endl;
		return false;
	}

	texture->w = surface->w;
	texture->h = surface->h;
	texture->clipX = 0;
	texture->clipY = 0;
	texture->clipW = 0;
	texture->clipH = 0;

	texture->flip = SDL_FLIP_NONE;
	strcpy(texture->name, name);
	SDL_FreeSurface(surface);

	return true;
}

//--------------------------------------------------------------------
bool Texture_LoadTexture(Texture* texture, SDL_Renderer* renderer, const char* path, const char* name, int x, int y, int w, int h) {
	if (!Texture_LoadTexture(texture, renderer, path, name)) {
		return false;
	}
	texture->clipX = x;
	texture->clipY = y;
	texture->clipW = w;
	texture->clipH = h;
	texture->w = texture->clipW;
	texture->h = texture->clipH;
	return true;
}


//--------------------------------------------------------------------
bool Texture_CreateTextureFromFont(Texture* texture, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, const char* message, const char* name) {
	if (!texture) {
		std::cerr << "Error: Uninitialized texture!" << std::endl;
		return false;
	}

	if (!font) {
		std::cerr << "Error: The font was nullptr!" << std::endl;
		return false;
	}

	SDL_Surface* surface = TTF_RenderText_Blended(font,message,color);

	if (surface == NULL) {
		std::cerr << "Could not load surface for message: " << message << "! Error: " << IMG_GetError() << std::endl;
		return false;
	}

	texture->sdltexture = SDL_CreateTextureFromSurface(renderer, surface);

	if (texture->sdltexture == NULL) {
		std::cerr << "Unable to create texture with message " << message << "! Error:" << IMG_GetError() << std::endl;
		return false;
	}

	texture->w = surface->w;
	texture->h = surface->h;
	texture->flip = SDL_FLIP_NONE;
	strcpy(texture->name, name);
	SDL_FreeSurface(surface);

	return true;
}

//--------------------------------------------------------------------
bool Texture_CreateTextureFromFontWithWidth(Texture* texture, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, const char* message, const char* name, int width) {
	if (!texture) {
		std::cerr << "Error: Uninitialized texture!" << std::endl;
		return false;
	}

	if (!font) {
		std::cerr << "Error: The font was nullptr!" << std::endl;
		return false;
	}

	SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, message, color, width);
	if (surface == NULL) {
		std::cerr << "Could not load surface for message: " << message << "! Error: " << IMG_GetError() << std::endl;
		return false;
	}

	texture->sdltexture = SDL_CreateTextureFromSurface(renderer, surface);

	if (texture->sdltexture == NULL) {
		std::cerr << "Unable to create texture with message " << message << "! Error:" << IMG_GetError() << std::endl;
		return false;
	}

	texture->w = surface->w;
	texture->h = surface->h;
	texture->flip = SDL_FLIP_NONE;
	strcpy(texture->name, name);
	SDL_FreeSurface(surface);

	return true;
}


//--------------------------------------------------------------------
bool Texture_CreateBlank(Texture* texture, SDL_Renderer* renderer) {
	// TO-DO: We may want a blank texture.
	(void) texture;
	(void) renderer;
	return true;
}


//--------------------------------------------------------------------
void Texture_Free(Texture* texture) {
	if (texture) {
		SDL_DestroyTexture(texture->sdltexture);
		texture = NULL;
	}
}





