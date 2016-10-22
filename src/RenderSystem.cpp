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
#include "HealthComponent.h"

#include <SDL.h>
#include <iostream>

// Render constants
const int XHealth_ = Constants::ScreenWidth_ - Constants::ScreenWidth_ / 4;
const int YHealth_ = Constants::ScreenHeight_ / 16;
const int WHealth_ = Constants::ScreenWidth_/ 5;
const int HHealth_ = Constants::ScreenHeight_ / 16;


// --------------------------------------------------------------------
void RenderSystem_Initialize(RenderSystem* renderSystem, ComponentBag* cBag, TileMap* tileMap) {
	renderSystem->textureComponent 		= cBag->textureComponent;
	renderSystem->rectangleComponent 	= cBag->rectangleComponent;
	renderSystem->animationComponent 	= cBag->animationComponent;
	renderSystem->movementComponent 	= cBag->movementComponent;
	renderSystem->cameraComponent 		= cBag->cameraComponent;
	renderSystem->hatComponent 		= cBag->hatComponent;
	renderSystem->map 			= tileMap;
	renderSystem->healthComponent 		= cBag->healthComponent;
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

	if (clip) {
		rquad.w = clip->w;
		rquad.h = clip->h;
	}

	SDL_RenderCopyEx(renderer, texture->sdltexture, clip, &rquad, 0.0, NULL, (SDL_RendererFlip)texture->flip);
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
	rquad.w = texture->w;
	rquad.h = texture->h;

	if (clip) {
		rquad.w = clip->w;
		rquad.h = clip->h;
	}

	if (!texture->sdltexture) {
		std::cerr << "Error: The sdl texture was null! Texture name: " << texture->name << "." << std::endl;
		return;
	}

	SDL_RenderCopyEx(renderer, texture->sdltexture, clip, &rquad, 0.0, NULL, (SDL_RendererFlip)texture->flip);
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
 	HealthComponent* healthComponent = renderSystem->healthComponent;


 	// Render background
 	Texture* background = TextureCache_GetTexture(Constants::GameBackground_);
	if (!background) {
		std::cerr << "Error: The game background is not available." << std::endl;
		return;
	}
	SDL_Rect backgroundClip = {cameraComponent->camera.x, cameraComponent->camera.y, Constants::ScreenWidth_, Constants::ScreenHeight_};
	RenderSystem_Render_xywh(renderer, 0, 0, background->w, background->h, &backgroundClip, background);
 	

 	// Render tile map
	Texture* tileset = TextureCache_GetTexture("tileset");
	int tileStartX = floor(cameraComponent->camera.x / Constants::TileSize_);
	int tileEndX = ceil((cameraComponent->camera.x + cameraComponent->camera.w) / Constants::TileSize_);
	int tileStartY = floor(cameraComponent->camera.y / Constants::TileSize_);
	int tileEndY = ceil((cameraComponent->camera.y + cameraComponent->camera.h) / Constants::TileSize_);
	if (tileset) {
		for (int r = tileStartY; r <= tileEndY; r++) {
			for (int c = tileStartX; c <= tileEndX; c++) {
				if (map->map[r][c].tid == 0) {
					continue;
				}
				int tid = map->map[r][c].tid - 1; // Minus zero to account for null tile
				int y = floor(tid / (tileset->w / Constants::TileSize_)) * Constants::TileSize_;
				int x = (tid % (tileset->w / Constants::TileSize_)) * Constants::TileSize_;
				SDL_Rect clip = {x, y, Constants::TileSize_, Constants::TileSize_};

				int xPos = c * Constants::TileSize_  - cameraComponent->camera.x;
				int yPos = r * Constants::TileSize_  - cameraComponent->camera.y;
				RenderSystem_Render_xywh(renderer, xPos, yPos, Constants::TileSize_, Constants::TileSize_, &clip, tileset);
			}
		}
	}


 	// Render entities
	for (uint32 texIndex = 0; texIndex < textureComponent->count; texIndex++) {
		uint32 eid = textureComponent->entityArray[texIndex];
		Texture* texture = textureComponent->textures[eid];

		// If no rectangle, render at (0,0)
		if (!Component_HasIndex(rectangleComponent, eid)) {
			Rectangle rect = {0, 0, texture->w, texture->h};
			RenderSystem_RenderCoord(renderer, &rect, NULL, texture);
			continue;
		}

		// Otherwise, render at the rectangle
		Rectangle rect = rectangleComponent->entityRectangles[eid];
		if (rect.x + rect.w < cameraComponent->camera.x || rect.x > cameraComponent->camera.x + cameraComponent->camera.w
			|| rect.y + rect.h < cameraComponent->camera.y || rect.y > cameraComponent->camera.y + cameraComponent->camera.h) {
			continue;
		}
		rect.x -= cameraComponent->camera.x;
		rect.y -= cameraComponent->camera.y;
		SDL_Rect clip = {0, 0, rect.w, rect.h};

		// Check for animation
		if (Component_HasIndex(animationComponent, eid)) {
			Animation* animation = &animationComponent->animations[eid];
			if (!animation) {
				std::cerr << "Error: The entity is supposed to have an animation, but none was found" << std::endl;
				continue;
		  	}

			// TO-DO: Move this out of render system. Not sure where yet.
			animation->currentFrameTime += delta;
			if (animation->currentFrameTime >= animation->frameTime) {
				animation->currentFrame++;
				animation->currentFrame %= animation->frames;
				animation->currentFrameTime = 0;
			}
			clip = {animation->spriteW * animation->currentFrame, 0, animation->spriteW, animation->spriteH};

			// Check for movement
			if (Component_HasIndex(movementComponent, eid)) {
				if (movementComponent->movementValues[eid].xVelocity == 0) {
					clip = {0, 0, animation->spriteW, animation->spriteH};
				} else if (movementComponent->movementValues[eid].xVelocity > 0) {
					texture->flip = SDL_FLIP_NONE;
				} else {
					texture->flip = SDL_FLIP_HORIZONTAL;
				}
			}	  
		}

		RenderSystem_RenderCoord(renderer, &rect, &clip, texture);

		// Check for hat
		if (Component_HasIndex(hatComponent, eid)) {
			Hat* hat = &hatComponent->hats[eid].hat;
			Hat* gHat = &hatComponent->hats[eid].gHat;
			if (strlen(gHat->name) > 1) {
				Texture* hatTexture = TextureCache_GetTexture(gHat->gname);
				if (hatTexture) {
					hatTexture->flip = texture->flip;
				  RenderSystem_Render_xywh(renderer, rect.x, rect.y - hatTexture->w / 2, hatTexture->w, hatTexture->h, NULL, hatTexture);
				}
			} else if (hat) {
			        Texture* hatTexture = TextureCache_GetTexture(hat->name);
				if (hatTexture) {
					hatTexture->flip = texture->flip;
				  RenderSystem_Render_xywh(renderer, rect.x, rect.y - hatTexture->w / 2, hatTexture->w, hatTexture->h, NULL, hatTexture);
				}
			}
		}
	}

	// Render HUD
	if (!Component_HasIndex(healthComponent, Constants::PlayerIndex_)) {
		std::cerr << "Error: The player has no renderable health component" << std::endl;
		return;
	}

	int max = healthComponent->maxHealth[Constants::PlayerIndex_];
	int current = healthComponent->health[Constants::PlayerIndex_];
	const SDL_Rect maxRect = {XHealth_, YHealth_, WHealth_, HHealth_};
	const SDL_Rect currentRect = {XHealth_, YHealth_, static_cast<int>(WHealth_ * ((float) current / max)), HHealth_};
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
	SDL_RenderFillRect(renderer, &maxRect);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 1);
	SDL_RenderFillRect(renderer, &currentRect);
}




