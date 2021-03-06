#include "RenderSystem.h"
#include "StatSystem.h"
#include "TextureCache.h"
#include "SoundCache.h"
#include "Types.h"
#include "ComponentBag.h"
#include "Game.h"
#include "Interactions.h"
#include "RectangleComponent.h"
#include "TextureComponent.h"
#include "AnimationComponent.h"
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
#include "BulletComponent.h"
#include "AliveComponent.h"

#include <cstring>
#include <math.h>
#include <SDL.h>
#include <iostream>

#define PI 3.14159265

// Render constants
const int XRightRender_ = Constants::ScreenWidth_ - Constants::ScreenWidth_ / 4 - 80;
const int XLeftRender_ = Constants::ScreenWidth_ / 24;
const int YTopRender_ = Constants::ScreenHeight_ / 16 + 3;
const int WHealth_ = 274;
const int HHealth_ = Constants::ScreenHeight_ / 16;
const int HealthBarHeight_ = 8;


// --------------------------------------------------------------------
void RenderSystem_Initialize(RenderSystem* renderSystem, ComponentBag* cBag, TileMap* tileMap, _TTF_Font* defaultFont) {
	renderSystem->textureComponent 		= cBag->textureComponent;
	renderSystem->rectangleComponent 	= cBag->rectangleComponent;
	renderSystem->animationComponent 	= cBag->animationComponent;
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
	                Texture* texture = TextureCache_GetTexture(Constants::BulletHud_);
			texture->flip = SDL_FLIP_NONE;
			int bulletIndex = 0;
			for (; bulletIndex < cBag->bulletComponent->bulletValues[Constants::PlayerIndex_].availableBullets; bulletIndex++) {
			  RenderSystem_Render_xywh(renderer, XRightRender_ + 289, YTopRender_ + ((texture->h + 4) * bulletIndex) + 63, texture->w, texture->h, NULL, texture); 
			}
			for (; bulletIndex < MaxBullets_; bulletIndex++) {
				uint32 eid = cBag->bulletComponent->bulletValues[Constants::PlayerIndex_].bulletEids[MaxBullets_ - bulletIndex - 1];
				if (cBag->aliveComponent->timeAlive[eid] > 0) {
					SDL_Rect clip = { 0, 0, static_cast<int>(texture->w * ((float)cBag->aliveComponent->timeAlive[eid] / MaxBulletLife_)), texture->h };
					RenderSystem_Render_xywh(renderer, XRightRender_ + 289, YTopRender_ + ((texture->h+4) * bulletIndex) + 63, texture->w, texture->h, &clip, texture); 
				}
			}
			break;
		}
        case HatTypes_Chef: {
	    Texture* texture = TextureCache_GetTexture(Constants::KnifeHud_);
			texture->flip = SDL_FLIP_NONE;
            for (int knifeIndex = 0; knifeIndex < cBag->bulletComponent->bulletValues[Constants::PlayerIndex_].availableBullets; knifeIndex++) {
	      RenderSystem_Render_xywh(renderer, XRightRender_ + 289, YTopRender_ + (texture->h + 6) * knifeIndex + 63, texture->w, texture->h, NULL, texture);
		}
		break;
	}
		default:
			break;
	}
}


// --------------------------------------------------------------------
void RenderSystem_Update(RenderSystem* renderSystem, SDL_Renderer* renderer, uint32 delta, int level) {
	TextureComponent* textureComponent = renderSystem->textureComponent;
 	RectangleComponent* rectangleComponent = renderSystem->rectangleComponent;
 	AnimationComponent* animationComponent = renderSystem->animationComponent;
 	MovementComponent* movementComponent = renderSystem->movementComponent;
 	CameraComponent* cameraComponent = renderSystem->cameraComponent;
 	HatComponent* hatComponent = renderSystem->hatComponent;
 	HealthComponent* healthComponent = renderSystem->healthComponent;
 	InteractableComponent* interactableComponent = renderSystem->interactableComponent;
 	GoalComponent* goalComponent = renderSystem->goalComponent;
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

		if (Component_HasIndex(healthComponent, eid) && healthComponent->invincible[eid] > 0) {
			healthComponent->invincible[eid] -= delta;
			if (healthComponent->invincible[eid] <= 0)
				healthComponent->invincible[eid] = 0;
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
					if (!ShouldDraw(eid, &healthComponent)) {
					  clip = {animation->spriteW * 6, 0, animation->spriteW, animation->spriteH};
					}
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
							texture->rotation += delta;
							if (texture->rotation > 360)
							  texture->rotation = 360;
						}
					} else {
						texture->rotation = 0;
					}
				}
			}
			if (!ShouldDraw(eid, &healthComponent))
			  animation->currentFrame -= 6;
		}

		//Adds a glow around glamour and regular hats
		//helps distinguish between them
		
		if (Component_HasIndex(interactableComponent, eid)) {
		  if (interactableComponent->types[eid] == 0) {
		    int ht = interactableComponent->datafield[eid];
		    Texture* glow;
		    switch (ht) {
		      case 0:
		      case 1:
		      case 5:
		      case 6:
		      case 8:
		        glow = TextureCache_GetTexture(Constants::HatGlow_);
			RenderSystem_Render_xywh(renderer, rect.x - (glow->w - rect.w)/2,
						 rect.y - (glow->h - rect.h)/2, glow->w, glow->h, NULL, glow);
		        break;
		      case 3:
		      case 4:
		      case 7:
		      case 9:
			glow = TextureCache_GetTexture(Constants::GlamourGlow_);
			RenderSystem_Render_xywh(renderer, rect.x - (rect.w - glow->w)/2,
						 rect.y - (-rect.h + glow->h)/2, glow->w, glow->h, NULL, glow);
		        break;
		    }
		    
		  }
		}
		
		RenderSystem_RenderCoord(renderer, &rect, &clip, texture);

		// Display interaction message
		if (Component_HasIndex(interactableComponent, eid)) {
			Texture* messageTexture;
			if (interactableComponent->canBeInteractedWith[eid]) {
				messageTexture = interactableComponent->msgs[eid];
				if (messageTexture){
					Rectangle messageRect = {rect.x + (rect.w / 2 - messageTexture->w / 2), rect.y - messageTexture->h, messageTexture->w, messageTexture->h};
					RenderSystem_RenderCoord(renderer, &messageRect, NULL, messageTexture);
				}
			}
		}

		if (Component_HasIndex(healthComponent, eid)) {
			if (eid != Constants::PlayerIndex_) {
				int current = HealthComponent_Lerp(healthComponent, eid, delta);
				int max = healthComponent->maxHealth[eid];
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

				gHatTexture->flip = textureComponent->textures[Constants::PlayerIndex_]->flip;
				gHatTexture->rotation = textureComponent->textures[Constants::PlayerIndex_]->rotation;
				
				float gX = rect.x + (rect.w - gHatTexture->w)/2;
				float gY = rect.y - gHatTexture->h / 2.5;
				float rads = (gHatTexture->rotation+90)*PI/180;
				float gXp = -rect.h*cos(rads)/2 + gX;
				float gYp = -rect.h*sin(rads)/2 + gY + rect.h/2;
				RenderSystem_Render_xywh(renderer, gXp, gYp, gHatTexture->w, gHatTexture->h, &clip, gHatTexture);
		    }
		    
		    Texture* pShader = TextureCache_GetTexture(Constants::PShader_);
		    char* name = (char*)hatComponent->hats[Constants::PlayerIndex_].gHat.name;
		    if (pShader && strcmp(name, "miner") != 0) {
		      RenderSystem_Render_xywh(renderer, rect.x + (rect.w - pShader->w)/2, rect.y + (rect.h - pShader->h)/2, pShader->w, pShader->h, NULL, pShader);
		    }
		    if (gHatTexture) {
		      RenderGlamourEffect(renderer, gHat->id, delta, &rect);
		    }
		    // Render HUD
		    if (Component_HasIndex(healthComponent, Constants::PlayerIndex_)) {
		      int current = HealthComponent_Lerp(healthComponent, Constants::PlayerIndex_, delta);
		      int max = healthComponent->maxHealth[Constants::PlayerIndex_];
		      //const SDL_Rect maxRect = {XRightRender_, YTopRender_, WHealth_, HHealth_};
		      const SDL_Rect currentRect = {XRightRender_ + 50, YTopRender_ - 9, static_cast<int>(WHealth_ * ((float) current / max)), HHealth_};
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
		      RenderSystem_Render_xywh(renderer, XRightRender_ - 23, YTopRender_ - 27, hBar->w, hBar->h, NULL, hBar);
		      SDL_SetRenderDrawColor(renderer, r, g, b, 1);
		      SDL_RenderFillRect(renderer, &currentRect);
		    }
	
		    if (Component_HasIndex(goalComponent, Constants::PlayerIndex_)) {
		      //Scoretime
		      scores[GameTime_] = goalComponent->points[Constants::PlayerIndex_];
		      int score = goalComponent->points[Constants::PlayerIndex_];
		      int seconds = ((int)(score / Constants::Second_)) % 60;
		      int minutes = ((int)(score / Constants::Second_)) / 60;


		      float xPos = (Constants::ScreenWidth_ - 120)/2 - 12;
		      Texture* tBar = TextureCache_GetTexture(Constants::TimeBar_);
		      RenderSystem_Render_xywh(renderer, xPos - 20, YTopRender_ - 44, tBar->w, tBar->h, NULL, tBar);

		      Texture* texture;
		      std::string bigKey;
		      std::string littleKey;

		      // Render Minutes
		      littleKey = std::to_string(minutes % 10);
		      minutes /= 10;
		      bigKey = std::to_string(minutes % 10);
		      texture = TextureCache_GetTexture(bigKey.c_str());
		      RenderSystem_Render_xywh(renderer, xPos, YTopRender_, texture->w, texture->h, NULL, texture);
		      xPos += texture->w;
		      texture = TextureCache_GetTexture(littleKey.c_str());
		      RenderSystem_Render_xywh(renderer, xPos, YTopRender_, texture->w, texture->h, NULL, texture);
		      xPos += texture->w;

		      // Render Colon
		      texture = TextureCache_GetTexture(":");
		      RenderSystem_Render_xywh(renderer, xPos, YTopRender_, texture->w, texture->h, NULL, texture);
		      xPos += TextureCache_GetTexture(":")->w;

		      // Render Seconds
		      littleKey = std::to_string(seconds % 10);
		      seconds /= 10;
		      bigKey = std::to_string(seconds % 10);
		      texture = TextureCache_GetTexture(bigKey.c_str());
		      RenderSystem_Render_xywh(renderer, xPos, YTopRender_, texture->w, texture->h, NULL, texture);
		      xPos += texture->w;
		      texture = TextureCache_GetTexture(littleKey.c_str());
		      RenderSystem_Render_xywh(renderer, xPos, YTopRender_, texture->w, texture->h, NULL, texture);

		      // Render Coins
		      Texture* coinTexture = TextureCache_GetTexture(Constants::CoinBar_);
		     if (coinTexture) {
				RenderSystem_Render_xywh(renderer, XLeftRender_ - 40, YTopRender_ - 30, coinTexture->w, coinTexture->h, NULL, coinTexture);
				Texture* firstNum = TextureCache_GetTexture(std::to_string(scores[Coins_]).c_str());
				Texture* lastNum = TextureCache_GetTexture(std::to_string(levelcoins[level]).c_str());
				Texture* backSlash = TextureCache_GetTexture("/");
				int startX = 47;
				if (firstNum) {
					RenderSystem_Render_xywh(renderer, XLeftRender_ + startX, YTopRender_ - 5, firstNum->w, firstNum->h, NULL, firstNum);
					startX += firstNum->w;
				}
				if (backSlash) {
					RenderSystem_Render_xywh(renderer, XLeftRender_ + startX, YTopRender_ - 5, backSlash->w, backSlash->h, NULL, backSlash);
					startX += backSlash->w;
				}
				if (lastNum) {
					RenderSystem_Render_xywh(renderer, XLeftRender_ + startX, YTopRender_ - 5, lastNum->w, lastNum->h, NULL, lastNum);
				}
		      }
		    }
		    if (gHatTexture) {
		      SDL_Rect clip = {0, 0, gHatTexture->w, gHatTexture->h};
		    	if (gHatTexture->clipW || gHatTexture->clipH) {
					clip = {gHatTexture->clipX, gHatTexture->clipY, gHatTexture->clipW, gHatTexture->clipH};
				}
		      gHatTexture->flip = SDL_FLIP_NONE;
		      gHatTexture->rotation = 0;
		      RenderSystem_Render_xywh(renderer, XRightRender_ + gHatTexture->w + 78, YTopRender_ + HHealth_ + 5, gHatTexture->w, gHatTexture->h, &clip, gHatTexture);
		    }
		    if (hatTexture) {
		      SDL_Rect clip = {0, 0, hatTexture->w, hatTexture->h};
	    	if (hatTexture->clipW || hatTexture->clipH) {
				clip = {hatTexture->clipX, hatTexture->clipY, hatTexture->clipW, hatTexture->clipH};
			}
		      hatTexture->flip = SDL_FLIP_NONE;
		      RenderSystem_Render_xywh(renderer, XRightRender_ + 50, YTopRender_ + HHealth_ + 7, hatTexture->w, hatTexture->h, &clip, hatTexture);
		    }
		    RenderHatHUD(renderer, hat->hatType, delta, renderSystem->cBag);
		}
	}

	//If credits level render

	/* Render credits. */
	if (level == Credits_){
	Texture* credits = TextureCache_GetTexture("credits");
	Texture* credits1 = TextureCache_GetTexture("harry");
	Texture* credits2 = TextureCache_GetTexture("matt");

	int renderX = (Constants::ScreenWidth_ / 8 )- credits->w / 2;
	RenderSystem_Render_xywh(renderer, renderX, ((0 * (Constants::ScreenHeight_ / 20)) + 450), credits->w, credits->h, NULL, credits);
	RenderSystem_Render_xywh(renderer, renderX, ((1 * (Constants::ScreenHeight_ / 20)) + 450), credits1->w, credits1->h, NULL, credits1);
	RenderSystem_Render_xywh(renderer, renderX, ((2 * (Constants::ScreenHeight_ / 20)) + 450), credits2->w, credits2->h, NULL, credits2);
	}
	
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
}

void RenderSystem_Free(RenderSystem* renderSystem) {
	renderSystem->textureComponent = nullptr;
	renderSystem->rectangleComponent = nullptr;
	renderSystem->animationComponent = nullptr;
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

	int invincible = healthComponent->invincible[eid];
	if (invincible > 0) {
		float ratio = ((float)invincible / Constants::InvincibleTime_);
		if (ratio > 0.9)
			return false;
		if (ratio <= 0.8 && ratio > 0.7)
			return false;
		if (ratio <= 0.6 && ratio > 0.5)
			return false;
		if (ratio <= 0.45 && ratio > 0.4)
			return false;
		if (ratio <= 0.35 && ratio > 0.3)
			return false;
		if (ratio <= 0.25 && ratio > 0.2)
			return false;
		if (ratio <= 0.15 && ratio > 0.1)
			return false;
		if (ratio <= 0.05 && ratio > 0)
			return false;
	}

 	return true;
}




