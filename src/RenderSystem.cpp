#include "RenderSystem.h"
#include "TextureCache.h"
#include "SoundCache.h"
#include "Types.h"
#include "ComponentBag.h"
#include "Game.h"
#include "Interactions.h"

#include "RectangleComponent.h"
#include "TextureComponent.h"
#include "AnimationComponent.h"
#include "BulletComponent.h"
#include "MovementComponent.h"
#include "CameraComponent.h"
#include "TileMap.h"
#include "HatComponent.h"
#include "HealthComponent.h"
#include "GlamourHatEnum.h"
#include "HatMethods.h"
#include "GoalComponent.h"
#include "InteractableComponent.h"
#include "NameComponent.h"
#include "AIComponent.h"

#include <cstring>
#include <math.h>
#include <SDL.h>
#include <iostream>

// Render constants
const int XRightRender_ = Constants::ScreenWidth_ - Constants::ScreenWidth_ / 4 - 80;
const int XLeftRender_ = Constants::ScreenWidth_ / 24;
const int YTopRender_ = Constants::ScreenHeight_ / 16 + 3;
const int WHealth_ = Constants::ScreenWidth_/ 5;
const int HHealth_ = Constants::ScreenHeight_ / 16;
const int HealthBarHeight_ = 8;


// --------------------------------------------------------------------
void RenderSystem_Initialize(RenderSystem* renderSystem, ComponentBag* cBag, TileMap* tileMap, _TTF_Font* defaultFont) {
	renderSystem->textureComponent 		= cBag->textureComponent;
	renderSystem->rectangleComponent 	= cBag->rectangleComponent;
	renderSystem->animationComponent 	= cBag->animationComponent;
	renderSystem->bulletComponent 		= cBag->bulletComponent;
	renderSystem->movementComponent 	= cBag->movementComponent;
	renderSystem->cameraComponent 		= cBag->cameraComponent;
	renderSystem->hatComponent 			= cBag->hatComponent;
	renderSystem->healthComponent 		= cBag->healthComponent;
	renderSystem->goalComponent 		= cBag->goalComponent;
	renderSystem->interactableComponent = cBag->interactableComponent;
	renderSystem->nameComponent 		= cBag->nameComponent;
	renderSystem->aiComponent               = cBag->aiComponent;
	renderSystem->defaultFont 		 	= defaultFont;
	renderSystem->map 					= tileMap;
	renderSystem->cBag 					= cBag;
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

	SDL_RenderCopyEx(renderer, texture->sdltexture, clip, &rquad, texture->rotation, NULL, (SDL_RendererFlip)texture->flip);
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

	SDL_RenderCopyEx(renderer, texture->sdltexture, clip, &rquad, texture->rotation, NULL, (SDL_RendererFlip)texture->flip);
}

// --------------------------------------------------------------------
void RenderGlamourEffect(SDL_Renderer* renderer, uint8 hatId, uint32 elapsed, Rectangle* rect) {
	switch (hatId) {
		case GlamourHatId_Disco:
			DiscoHatUpdate(elapsed);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, DiscoHat::r, DiscoHat::g, DiscoHat::b, DiscoHat::a);
			SDL_RenderFillRect(renderer, &DiscoHat::Rectangle);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
			break;
	    case GlamourHatId_Miner: {
		    	Texture* mShader = TextureCache_GetTexture("miner-shader");
		    	mShader->flip = TextureCache_GetTexture("miner")->flip;
			RenderSystem_Render_xywh(renderer, rect->x + (rect->w - mShader->w)/2, rect->y + (rect->h- mShader->h)/ 2, mShader->w, mShader->h, NULL, mShader);
	      		break;
	      	}
      	default:
      		break;
		
	}
}

// --------------------------------------------------------------------
void RenderHatHUD(SDL_Renderer* renderer, uint hatId, uint32 elapsed, ComponentBag* cBag) {
	(void) cBag;
	(void) elapsed;
	switch (hatId) {
	        case HatTypes_Cowboy: {
			Texture* texture = TextureCache_GetTexture("bullet");
			texture->flip = SDL_FLIP_NONE;
			for (int bulletIndex = CowboyHat::bulletCount; bulletIndex < Constants::MaxBullets_; bulletIndex++) {
			      	RenderSystem_Render_xywh(renderer, XLeftRender_, YTopRender_ + (texture->h * bulletIndex), texture->w, texture->h, NULL, texture); 
			}
			break;
		}
	        case HatTypes_Chef: {
	                Texture* texture = TextureCache_GetTexture("knife");
			texture->flip = SDL_FLIP_NONE;
	                for (int knifeIndex = ChefHat::knifeCount; knifeIndex < Constants::MaxKnives_; knifeIndex++) {
	                        RenderSystem_Render_xywh(renderer, XLeftRender_, YTopRender_ + texture->h * knifeIndex,
							 texture->w, texture->h, NULL, texture);
			}
			break;
		}
		default:
			break;
	}
}


// --------------------------------------------------------------------
void RenderSystem_Update(RenderSystem* renderSystem, SDL_Renderer* renderer, uint32 delta) {
	TextureComponent* textureComponent = renderSystem->textureComponent;
 	RectangleComponent* rectangleComponent = renderSystem->rectangleComponent;
 	AnimationComponent* animationComponent = renderSystem->animationComponent;
 	//BulletComponent* bulletComponent = renderSystem->bulletComponent;
 	MovementComponent* movementComponent = renderSystem->movementComponent;
 	CameraComponent* cameraComponent = renderSystem->cameraComponent;
 	HatComponent* hatComponent = renderSystem->hatComponent;
 	HealthComponent* healthComponent = renderSystem->healthComponent;
 	InteractableComponent* interactableComponent = renderSystem->interactableComponent;
 	//GoalComponent* goalComponent = renderSystem->goalComponent;
 	NameComponent* nameComponent = renderSystem->nameComponent;
	AIComponent* aiComponent = renderSystem->aiComponent;
 	TileMap* map = renderSystem->map;


 	// Render background
 	Texture* background = TextureCache_GetTexture(Constants::GameBackground_);
	if (!background) {
		std::cerr << "Error: The game background is not available." << std::endl;
		return;
	}
	SDL_Rect backgroundClip = {cameraComponent->camera.x / 2, (cameraComponent->camera.y), Constants::ScreenWidth_, Constants::ScreenHeight_};
	RenderSystem_Render_xywh(renderer, 0, 0, background->w, background->h, &backgroundClip, background);
 	

 	// Render tile map
	Texture* tileset = TextureCache_GetTexture(Constants::TilesetName_);
	int tileStartX = floor(cameraComponent->camera.x / Constants::TileSize_);
	int tileEndX = ceil((cameraComponent->camera.x + cameraComponent->camera.w) / Constants::TileSize_);
	int tileStartY = floor(cameraComponent->camera.y / Constants::TileSize_);
	int tileEndY = ceil((cameraComponent->camera.y + cameraComponent->camera.h) / Constants::TileSize_);
	if (tileset) {
		for (int r = tileStartY; r <= tileEndY; r++) {
			for (int c = tileStartX; c <= tileEndX; c++) {
				if (map->map[r][c].tid[0] == 0) {
					continue;
				}

				Tile* tile = &map->map[r][c];
				int tid = 0;
				if (tile->animationTime == 0) {
					tid = tile->tid[0] - 1;
				} else {
					tile->elapsed += delta;
					if (tile->elapsed >= tile->animationTime) {
						tile->elapsed = 0;
						tile->currentIndex++;
						if (tile->tid[tile->currentIndex] == 0) {
							tile->currentIndex = 0;
						}
					}
					tid = tile->tid[tile->currentIndex] - 1;
				}

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
		if (!Component_HasIndex(textureComponent, eid)) {
			continue;
		}
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
			|| rect.y > cameraComponent->camera.y + cameraComponent->camera.h || rect.y + rect.h < cameraComponent->camera.y) {
			continue;
		}
		rect.x -= cameraComponent->camera.x;
		rect.y -= cameraComponent->camera.y;
		SDL_Rect clip = {0, 0, rect.w, rect.h};

		if (texture->clipW || texture->clipH) {
			clip = {texture->clipX, texture->clipY, texture->clipW, texture->clipH};
		}

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
			if (!ShouldDraw(eid, &healthComponent))
			  animation->currentFrame += 6;
			clip = {animation->spriteW * animation->currentFrame, 0, animation->spriteW, animation->spriteH};

			// Check for movement
			if (Component_HasIndex(movementComponent, eid)) {
				if (movementComponent->movementValues[eid].xVelocity == 0) {
				        if (Component_HasIndex(aiComponent, eid) && aiComponent->marchValues[eid].facing < 0) {
				            texture->flip = SDL_FLIP_HORIZONTAL;
				        } else if (Component_HasIndex(aiComponent, eid) && aiComponent->marchValues[eid].facing > 0) {
					  texture->flip = SDL_FLIP_NONE;
					}
					clip = {0, 0, animation->spriteW, animation->spriteH};
				} else if (movementComponent->movementValues[eid].xVelocity > 0) {
					texture->flip = SDL_FLIP_NONE;
					movementComponent->movementValues[eid].left = false;
				} else {
					texture->flip = SDL_FLIP_HORIZONTAL;
					movementComponent->movementValues[eid].left = true; //facing left
				}

				if (eid == Constants::PlayerIndex_) {
					if (!movementComponent->movementValues[eid].grounded) {
						int invc = 0;
						if (!ShouldDraw(eid, &healthComponent))
							invc = 6;
						if (movementComponent->movementValues[eid].yVelocity > 0)
							clip = {animation->spriteW * (5+invc), 0, animation->spriteW, animation->spriteH};
						else
							clip = {animation->spriteW * (4+invc), 0, animation->spriteW, animation->spriteH};

						if (hatComponent->hats[eid].gHat.hatType == HatTypes_Circus) {
							texture->rotation += Constants::OptimalTime_ / 5;
						}
					} else {
						texture->rotation = 0;
					}
				}
			}
			
			if (!ShouldDraw(eid, &healthComponent))
			  animation->currentFrame -= 6;
		}
		
		  RenderSystem_RenderCoord(renderer, &rect, &clip, texture);

		// Display interaction message
		if (Component_HasIndex(interactableComponent, eid)) {
			Texture* messageTexture;
			if (interactableComponent->canBeInteractedWith[eid]) {
				messageTexture = interactableComponent->msgs[eid];
				Rectangle messageRect = {rect.x + (rect.w / 2 - messageTexture->w / 2), rect.y - messageTexture->h, messageTexture->w, messageTexture->h};
				RenderSystem_RenderCoord(renderer, &messageRect, NULL, messageTexture);
			}
		}

		if (Component_HasIndex(healthComponent, eid)) {
			if (eid != Constants::PlayerIndex_) {
				int max = healthComponent->maxHealth[eid];
				int current = healthComponent->health[eid];
				if (current < 0) {
				  current = 0;
				  healthComponent->health[eid] = 0;
				}
				//const SDL_Rect maxRect = {XRightRender_, YTopRender_, static_cast<int>(rect.w), HealthBarHeight_};
				const SDL_Rect currentRect = {static_cast<int>(rect.x), static_cast<int>(rect.y) - HealthBarHeight_, static_cast<int>(rect.w * ((float) current / max)), HealthBarHeight_};
			        float ratio = (float) current / max;
				int r = 167;
				int g = 255;
				int b = 131;
				int r2 = 255;
				int g2 = 92;
				int b2 = 78;
				r = r2 + (r - r2)*ratio;
				g = g2 + (g - g2)*ratio;
				b = b2 + (b - b2)*ratio;
				SDL_SetRenderDrawColor(renderer, r, g, b, 1);
				SDL_RenderFillRect(renderer, &currentRect);
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
			}
		}
		if (Component_HasIndex(nameComponent, eid)) {
			Texture* nameTexture = nameComponent->textures[eid];
			if (nameTexture) {
				Rectangle nameRect = {rect.x + (rect.w / 2 - nameTexture->w / 2), rect.y - nameTexture->h - HealthBarHeight_, nameTexture->w, nameTexture->h};
				RenderSystem_RenderCoord(renderer, &nameRect, NULL, nameTexture);
			}
		}
	} // End entity render.
	
	// Render hats
	if (Component_HasIndex(hatComponent, Constants::PlayerIndex_)) {
		Hat* hat = &hatComponent->hats[Constants::PlayerIndex_].hat;
		Hat* gHat = &hatComponent->hats[Constants::PlayerIndex_].gHat;
		Texture* gHatTexture = TextureCache_GetTexture(gHat->name);
	  	Texture* hatTexture = TextureCache_GetTexture(hat->name);

		if (Component_HasIndex(rectangleComponent, Constants::PlayerIndex_)) {
		    Rectangle rect = rectangleComponent->entityRectangles[Constants::PlayerIndex_];
		    rect.x -= cameraComponent->camera.x;
		    rect.y -= cameraComponent->camera.y;
		    if (hatTexture) {
		    	SDL_Rect clip = {0, 0, hatTexture->w, hatTexture->h};
		    	if (hatTexture->clipW || hatTexture->clipH) {
					clip = {hatTexture->clipX, hatTexture->clipY, hatTexture->clipW, hatTexture->clipH};
				}
				hatTexture->flip = SDL_FLIP_NONE;
				RenderSystem_Render_xywh(renderer, XRightRender_, YTopRender_ + HHealth_ + 10, hatTexture->w, hatTexture->h, &clip, hatTexture);
				hatTexture->flip = textureComponent->textures[Constants::PlayerIndex_]->flip;
				if (!gHatTexture || strcmp(gHatTexture->name, "") == 0) {
					RenderSystem_Render_xywh(renderer, rect.x + (rect.w - hatTexture->w)/2, rect.y - hatTexture->h / 2.5, hatTexture->w, hatTexture->h, &clip, hatTexture);
				}
		    }
		    if (gHatTexture) {
		    	SDL_Rect clip = {0, 0, gHatTexture->w, gHatTexture->h};
		    	if (gHatTexture->clipW || gHatTexture->clipH) {
					clip = {gHatTexture->clipX, gHatTexture->clipY, gHatTexture->clipW, gHatTexture->clipH};
				}
				gHatTexture->flip = SDL_FLIP_NONE;
				RenderSystem_Render_xywh(renderer, XRightRender_ + gHatTexture->w + 10, YTopRender_ + HHealth_ + 10, gHatTexture->w, gHatTexture->h, &clip, gHatTexture);
				gHatTexture->flip = textureComponent->textures[Constants::PlayerIndex_]->flip;
				RenderSystem_Render_xywh(renderer, rect.x + (rect.w - gHatTexture->w)/2, rect.y - gHatTexture->h / 2.5, gHatTexture->w, gHatTexture->h, &clip, gHatTexture);
		    }
		    
		    Texture* pShader = TextureCache_GetTexture(Constants::PShader_);
		    char* name = (char*)hatComponent->hats[Constants::PlayerIndex_].gHat.name;
		    if (pShader && strcmp(name, "miner") != 0) {
		      RenderSystem_Render_xywh(renderer, rect.x + (rect.w - pShader->w)/2, rect.y + (rect.h - pShader->h)/2, pShader->w, pShader->h, NULL, pShader);
		    }

		    if (gHat) {
		      RenderGlamourEffect(renderer, gHat->id, delta, &rect);
		    }
		    RenderHatHUD(renderer, hat->hatType, delta, renderSystem->cBag);
		}
	}
	
	// Render HUD
	if (Component_HasIndex(healthComponent, Constants::PlayerIndex_)) {
		int current = HealthComponent_Lerp(healthComponent, Constants::PlayerIndex_, delta);
		int max = healthComponent->maxHealth[Constants::PlayerIndex_];
		//const SDL_Rect maxRect = {XRightRender_, YTopRender_, WHealth_, HHealth_};
		const SDL_Rect currentRect = {XRightRender_, YTopRender_, static_cast<int>(WHealth_ * ((float) current / max)), HHealth_};
		float ratio = (float) current / max;
		int r = 167;
		int g = 255;
		int b = 131;
		int r2 = 255;
		int g2 = 92;
		int b2 = 78;
		r = r2 + (r - r2)*ratio;
		g = g2 + (g - g2)*ratio;
		b = b2 + (b - b2)*ratio;
		Texture* hBar = TextureCache_GetTexture(Constants::HealthBar_);
		RenderSystem_Render_xywh(renderer, XRightRender_ - 8, YTopRender_ - 44, hBar->w, hBar->h, NULL, hBar);
		SDL_SetRenderDrawColor(renderer, r, g, b, 1);
		SDL_RenderFillRect(renderer, &currentRect);
	}

	/*if (Component_HasIndex(goalComponent, Constants::PlayerIndex_)) {
		Texture scoreTexture;
		int score = goalComponent->points[Constants::PlayerIndex_];
		std::string scoreStr = std::to_string(score / Constants::Second_);
		Texture_CreateTextureFromFont(&scoreTexture, renderer, renderSystem->defaultFont, scoreColor, scoreStr.substr(0, 4).c_str(), "score_string");
		RenderSystem_Render_xywh(renderer, XLeftRender_, YTopRender_, scoreTexture.w, scoreTexture.h, NULL, &scoreTexture);
	}*/
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
}

void RenderSystem_Free(RenderSystem* renderSystem) {
	renderSystem->textureComponent = nullptr;
	renderSystem->rectangleComponent = nullptr;
	renderSystem->animationComponent = nullptr;
	renderSystem->bulletComponent = nullptr;
	renderSystem->movementComponent = nullptr;
	renderSystem->cameraComponent = nullptr;
	renderSystem->hatComponent = nullptr;
	renderSystem->healthComponent = nullptr;
	renderSystem->goalComponent = nullptr;
	renderSystem->interactableComponent = nullptr;
	renderSystem->map = nullptr;
	renderSystem->defaultFont = nullptr;
}

bool ShouldDraw(uint32 eid, HealthComponent** hComponent) {
  HealthComponent* healthComponent = *hComponent;
  if (eid != Constants::PlayerIndex_ || !Component_HasIndex(healthComponent, eid))
    return true;
  if (healthComponent->invincible[eid] <= 0)
    return true;
  if (healthComponent->invincible[eid] <= 20)
    return false;
  if (healthComponent->invincible[eid] <= 40)
    return true;
  if (healthComponent->invincible[eid] <= 60)
    return false;
  if (healthComponent->invincible[eid] <= 80)
    return true;
  if (healthComponent->invincible[eid] <= 160)
    return false;
  if (healthComponent->invincible[eid] <= 240)
    return true;
  if (healthComponent->invincible[eid] <= 320)
    return false;
  if (healthComponent->invincible[eid] <= 400)
    return true;
  if (healthComponent->invincible[eid] <= 480)
    return false;
  if (healthComponent->invincible[eid] <= 560)
    return true;
  if (healthComponent->invincible[eid] <= 640)
    return false;
  return true;
}




