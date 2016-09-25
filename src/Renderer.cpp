#include "Texture.h"
#include "Renderer.h"
#include "Coord2D.h"
#include <SDL.h>

bool Renderer_Initialize(Renderer* renderer, SDL_Window* window) {
	renderer->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer->renderer) {
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	return true;
}

void Renderer_RenderCoord(Renderer* renderer, Coord2D* point, Texture* texture) {
	if (!renderer) {
		return;
	}
	if (!texture) {
		return;
	}

	SDL_Rect rquad;
	rquad.x = point->x;
	rquad.y = point->y;
	rquad.w = texture->w;
	rquad.h = texture->h;
	SDL_RenderCopy(renderer->renderer, texture->sdltexture, NULL, &rquad);
}

void Renderer_CompleteRender(Renderer* renderer) {
	SDL_RenderPresent(renderer->renderer);
}

void Renderer_Free(Renderer* renderer) {
	if (renderer) {
		SDL_DestroyRenderer(renderer->renderer);
	}
}