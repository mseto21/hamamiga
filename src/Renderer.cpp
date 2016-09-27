#include "Texture.h"
#include "Renderer.h"
#include "Coord2D.h"
#include <SDL.h>
#include <iostream>

void Renderer_RenderCoord(SDL_Renderer* renderer, Coord2D* point, Texture* texture) {
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

	SDL_RenderCopy(renderer, texture->sdltexture, NULL, &rquad);
}

void Renderer_Free(SDL_Renderer* renderer) {
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
}