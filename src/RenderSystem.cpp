#include "Texture.h"
#include "RenderSystem.h"
#include "Types.h"
#include "RectangleComponent.h"
#include "TextureComponent.h"
#include "AnimationComponent.h"
#include "MovementComponent.h"
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
void RenderSystem_RenderCoord(SDL_Renderer* renderer, Rectangle* rect, SDL_Rect* clip, Texture* texture, int flip) {
	if (!renderer) {
		std::cerr << "Error: The renderer was null!" << std::endl;
		return;
	}
	if (!texture) {
		std::cerr << "Error: The given texture was null!" << std::endl;
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

	SDL_RenderCopyEx(renderer, texture->sdltexture, clip, &rquad, 0.0, NULL, (SDL_RendererFlip)flip);
}

// --------------------------------------------------------------------
void RenderSystem_Update(SDL_Renderer* renderer, float delta, TextureComponent* textureComponent, RectangleComponent* rectangleComponent, AnimationComponent* animationComponent, MovementComponent* movementComponent) {
	for (uint32 texIndex = 0; texIndex < textureComponent->count; texIndex++) {
		uint32 eid = textureComponent->entityArray[texIndex];
		Texture* texture = textureComponent->textures[eid];
		SDL_RendererFlip flip = SDL_FLIP_NONE;
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
				if (Component_HasIndex(movementComponent, eid)) {
					if (movementComponent->movementValues[eid].xVelocity < 0) flip = SDL_FLIP_HORIZONTAL;
				}
				SDL_Rect clip = {animation->spriteW * animation->currentFrame, 0, animation->spriteW, animation->spriteH};
				RenderSystem_RenderCoord(renderer, rect, &clip, texture, flip);
			} else {
				RenderSystem_RenderCoord(renderer, rect, NULL, texture, flip);
			}
			continue;
		}

		Rectangle rect = {0, 0, texture->w, texture->h};
		RenderSystem_RenderCoord(renderer, &rect, NULL, texture, SDL_FLIP_NONE);
	}
}




