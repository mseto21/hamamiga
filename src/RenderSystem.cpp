#include "Texture.h"
#include "Renderer.h"
#include "Coord2D.h"
#include "Rectangle.h"
#include <SDL.h>
#include <iostream>

void Renderer_RenderCoord(SDL_Renderer* renderer, Rectangle* rect, Texture* texture) {
	if (!renderer) {
		return;
	}
	if (!texture) {
		return;
	}

	SDL_Rect rquad;
	rquad.x = rect->x;
	rquad.y = rect->y;
	rquad.w = rect->w;
	rquad.h = rect->h;

	SDL_RenderCopy(renderer, texture->sdltexture, NULL, &rquad);
}

void Renderer_Update(RendereComponent* renderComponent) {
	
}

void Renderer_Free(SDL_Renderer* renderer) {
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
}