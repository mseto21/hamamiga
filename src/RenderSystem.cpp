#include "Texture.h"
#include "RenderSystem.h"
#include "Types.h"
#include "RectangleComponent.h"
#include "TextureComponent.h"
#include "AnimationComponent.h"
#include <SDL.h>
#include <iostream>

void RenderSystem_Render_xywh(SDL_Renderer* renderer, int x, int y, int w, int h, Texture* texture) {
	if (!renderer) {
		return;
	}
	if (!texture) {
		return;
	}

	SDL_Rect rquad;
	rquad.x = x;
	rquad.y = y;
	rquad.w = w;
	rquad.h = h;

	SDL_RenderCopy(renderer, texture->sdltexture, NULL, &rquad);
}

// --------------------------------------------------------------------
void RenderSystem_RenderCoord(SDL_Renderer* renderer, Rectangle* rect, SDL_Rect* clip, Texture* texture) {
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

	if (clip) {
		rquad.w = clip->w;
		rquad.h = clip->h;
	}

	SDL_RenderCopy(renderer, texture->sdltexture, clip, &rquad);
}

// --------------------------------------------------------------------
void RenderSystem_Update(SDL_Renderer* renderer, float delta, TextureComponent* textureComponent, RectangleComponent* rectangleComponent, AnimationComponent* animationComponent) {
	SDL_RenderClear(renderer);
	for (uint32 texIndex = 0; texIndex < textureComponent->count; texIndex++) {
		uint32 eid = textureComponent->entityArray[texIndex];
		Texture* texture = textureComponent->textures[eid];

		if (Component_HasIndex(rectangleComponent, eid)) {
			Rectangle* rect = &rectangleComponent->entityRectangles[eid];

			if (Component_HasIndex(animationComponent, eid)) {
				Animation* animation = &animationComponent->animations[eid];
				animation->currentFrameTime += delta;
				if (animation->currentFrameTime > animation->frameTime) {
					animation->currentFrame++;
					animation->currentFrame %= animation->frames;
					animation->currentFrameTime = 0;
				}
				SDL_Rect clip = {animation->spriteW * animation->currentFrame, 0, animation->spriteW, animation->spriteH};
				RenderSystem_RenderCoord(renderer, rect, &clip, texture);
			} else {
				RenderSystem_RenderCoord(renderer, rect, NULL, texture);
			}
			continue;
		}

		Rectangle rect = {0, 0, texture->w, texture->h};
		RenderSystem_RenderCoord(renderer, &rect, NULL, texture);
	}
	SDL_RenderPresent(renderer);
}

// --------------------------------------------------------------------
void RenderSystem_Free(SDL_Renderer* renderer) {
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
}