#include "RenderSystem.h"
#include "Types.h"
#include "RectangleComponent.h"
#include "TextureComponent.h"
#include "AnimationComponent.h"
#include "MovementComponent.h"
#include "CameraComponent.h"
#include "TextureCache.h"
#include "TileMap.h"
#include <SDL.h>
#include <iostream>

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
		std::cerr << "Error: The sdl texture was null!" << std::endl;
		return;
	}

	SDL_RenderCopyEx(renderer, texture->sdltexture, clip, &rquad, 0.0, NULL, SDL_FLIP_NONE);
}

// --------------------------------------------------------------------
void RenderSystem_Update(SDL_Renderer* renderer, float delta, TextureComponent* textureComponent,
 RectangleComponent* rectangleComponent, AnimationComponent* animationComponent, MovementComponent* movementComponent,
 CameraComponent* cameraComponent, TileMap* map) {
 	
 	// Render tile map
	Texture* tileset = TextureCache_GetTexture("tileset");
	for (int r = 0; r < Constants::MaxMapHeight_; r++) {
		for (int c = 0; c < Constants::MaxMapWidth_; c++) {
			if (map->map[r][c].tid == 0) {
				continue;
			}
			int tid = map->map[r][c].tid;
			int mapWidth = Constants::LevelWidth_ / Constants::TileSize_;
			int mapHeight = Constants::LevelHeight_ / Constants::TileSize_;
			int x = floor(tid / mapWidth) * Constants::TileSize_;
			int y = (tid % mapHeight) * Constants::TileSize_;
			SDL_Rect clip = {x, y, Constants::TileSize_, Constants::TileSize_};
			//std::cout << x << ", " << y << std::endl;
			RenderSystem_Render_xywh(renderer, r * Constants::TileSize_, c * Constants::TileSize_, Constants::TileSize_, Constants::TileSize_, &clip, tileset);
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
					
				}
				SDL_Rect clip = {animation->spriteW * animation->currentFrame, 0, animation->spriteW, animation->spriteH};
				RenderSystem_RenderCoord(renderer, &rect, &clip, texture);
			} else {
				RenderSystem_RenderCoord(renderer, &rect, NULL, texture);
			}
			continue;
		}

		Rectangle rect = {0, 0, texture->w, texture->h};
		RenderSystem_RenderCoord(renderer, &rect, NULL, texture);
	}
}




