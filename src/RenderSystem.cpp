#include "RenderSystem.h"
#include "TextureCache.h"
#include "Types.h"
#include "ComponentBag.h"

#include "RectangleComponent.h"
#include "TextureComponent.h"
#include "AnimationComponent.h"
#include "MovementComponent.h"
#include "CameraComponent.h"
#include "TileMap.h"
#include "HatComponent.h"

#include <SDL.h>
#include <iostream>


// --------------------------------------------------------------------
void RenderSystem_Initialize(RenderSystem* renderSystem, ComponentBag* cBag, TileMap* tileMap) {
	renderSystem->textureComponent 		= cBag->textureComponent;
	renderSystem->rectangleComponent 	= cBag->rectangleComponent;
	renderSystem->animationComponent 	= cBag->animationComponent;
	renderSystem->movementComponent 	= cBag->movementComponent;
	renderSystem->cameraComponent 		= cBag->cameraComponent;
	renderSystem->hatComponent 			= cBag->hatComponent;
	renderSystem->map 					= tileMap;
}


// --------------------------------------------------------------------
void RenderSystem_Render_xywh(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Rect* clip, Texture* texture) {
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

	SDL_RenderCopy(renderer, texture->sdltexture, clip, &rquad);
}

// --------------------------------------------------------------------
void RenderSystem_RenderCoord(SDL_Renderer* renderer, Rectangle* rect, SDL_Rect* clip, Texture* texture) {
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

	if (!texture->sdltexture) {
		std::cerr << "Error: The sdl texture was null! Texture name: " << texture->name << "." << std::endl;
		return;
	}

	SDL_RenderCopyEx(renderer, texture->sdltexture, clip, &rquad, 0.0, NULL, SDL_FLIP_NONE);
}

// --------------------------------------------------------------------
void RenderSystem_Update(RenderSystem* renderSystem, SDL_Renderer* renderer, uint32 delta) {
	TextureComponent* textureComponent = renderSystem->textureComponent;
 	RectangleComponent* rectangleComponent = renderSystem->rectangleComponent;
 	AnimationComponent* animationComponent = renderSystem->animationComponent;
 	MovementComponent* movementComponent = renderSystem->movementComponent;
 	CameraComponent* cameraComponent = renderSystem->cameraComponent;
 	HatComponent* hatComponent = renderSystem->hatComponent;
 	TileMap* map = renderSystem->map;

 	SDL_RenderClear(renderer);

 	// Render background
 	Texture* background = TextureCache_GetTexture(Constants::GameBackground_);
	if (!background) {
		std::cerr << "Error: The game background is not available." << std::endl;
		return;
	}
	RenderSystem_Render_xywh(renderer, -cameraComponent->camera.x, -cameraComponent->camera.y, background->w, background->h, NULL, background);
 	
 	// Render tile map
	Texture* tileset = TextureCache_GetTexture("tileset");
	for (int r = 0; r <= map->h; r++) {
		for (int c = 0; c <= map->w; c++) {
			if (map->map[r][c].tid == 0) {
				continue;
			}
			int tid = map->map[r][c].tid - 1; // Minus zero to account for null tile
			int y = floor(tid / (tileset->w / Constants::TileSize_)) * Constants::TileSize_;
			int x = (tid % (tileset->w / Constants::TileSize_)) * Constants::TileSize_;
			SDL_Rect clip = {x, y, Constants::TileSize_, Constants::TileSize_};
			RenderSystem_Render_xywh(renderer, c * Constants::TileSize_  - cameraComponent->camera.x, r * Constants::TileSize_  - cameraComponent->camera.y, Constants::TileSize_, Constants::TileSize_, &clip, tileset);
		}
	}

 	// Render entities
	for (uint32 texIndex = 0; texIndex < textureComponent->count; texIndex++) {
		uint32 eid = textureComponent->entityArray[texIndex];
		Texture* texture = textureComponent->textures[eid];
		if (Component_HasIndex(rectangleComponent, eid)) {
			Rectangle rect = rectangleComponent->entityRectangles[eid];
			rect.x -= cameraComponent->camera.x;
			rect.y -= cameraComponent->camera.y;

			// Check for animation
			if (Component_HasIndex(animationComponent, eid)) {
				Animation* animation = &animationComponent->animations[eid];
				if (!animation) {
					std::cerr << "Error: The entity is supposed to have an animation, but none was found" << std::endl;
					continue;
				}
				animation->currentFrameTime += delta;
				if (animation->currentFrameTime >= animation->frameTime) {
					animation->currentFrame++;
					animation->currentFrame %= animation->frames;
					animation->currentFrameTime = 0;
				}
				if (Component_HasIndex(movementComponent, eid)) {
					// TO-DO: Flip
				}
				SDL_Rect clip = {animation->spriteW * animation->currentFrame, 0, animation->spriteW, animation->spriteH};
				RenderSystem_RenderCoord(renderer, &rect, &clip, texture);
			} else {
				RenderSystem_RenderCoord(renderer, &rect, NULL, texture);
			}

			// Check for hat
			if (Component_HasIndex(hatComponent, eid)) {
				Hat* hat = &hatComponent->hats[eid].hat;
				if (hat) {
					Texture* hatTexture = TextureCache_GetTexture(hat->name);
					if (hatTexture) {
						RenderSystem_Render_xywh(renderer, rect.x, rect.y, hatTexture->w, hatTexture->h, NULL, hatTexture);
					}
				}
			}

			continue;
		}

		// If no rectangle, render at (0,0)
		Rectangle rect = {0, 0, texture->w, texture->h};
		RenderSystem_RenderCoord(renderer, &rect, NULL, texture);
	}

	
	SDL_RenderPresent(renderer);
}




