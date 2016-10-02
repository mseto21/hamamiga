#include "Texture.h"
#include "Renderer.h"
#include "Coord2D.h"
#include <SDL.h>
#include <iostream>

void Renderer_RenderCoord(SDL_Renderer* renderer, Coord2D* point, Texture* texture, SDL_Rect* clip) {
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

	if( clip != NULL )
	{
		rquad.w = clip->w;
		rquad.h = clip->h;
	}


	SDL_RenderCopy(renderer, texture->sdltexture, clip, &rquad);
}

void Renderer_Free(SDL_Renderer* renderer) {
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
}