#include "PhysicsSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "PhysicsComponent.h"
#include "RectangleComponent.h"
#include "HealthComponent.h"
#include "HatComponent.h"
#include "InputComponent.h"
#include "AliveComponent.h"
#include "GoalComponent.h"
#include "InteractableComponent.h"
#include "TileMap.h"
#include "ComponentBag.h"
#include "SoundCache.h"
#include "InteractionTypes.h"

#include <stdlib.h>
#include <SDL.h>
#include <iostream>

void PhysicsSystem_Initialize(PhysicsSystem* physicsSystem, ComponentBag* cBag, TileMap* tileMap) {
	physicsSystem->physicsComponent 	= cBag->physicsComponent;
	physicsSystem->movementComponent 	= cBag->movementComponent;
	physicsSystem->rectangleComponent 	= cBag->rectangleComponent;
	physicsSystem->healthComponent 		= cBag->healthComponent;
	physicsSystem->hatComponent 		= cBag->hatComponent;
	physicsSystem->inputComponent		= cBag->inputComponent;
	physicsSystem->goalComponent 		= cBag->goalComponent;
	physicsSystem->interactableComponent = cBag->interactableComponent;
	physicsSystem->aliveComponent 		= cBag->aliveComponent;
	physicsSystem->map 					= tileMap;
	physicsSystem->componentBag 		= cBag;
}


bool Collision(const Rectangle r1, const Rectangle r2) {
  return r1.x <= r2.x + r2.w 
  		&& r1.x + r1.w >= r2.x 
  		&& r1.y <= r2.y + r2.h 
  		&& r1.y + r1.h >= r2.y;
}


void PhysicsSystem_Update(PhysicsSystem* physicsSystem) {
	PhysicsComponent* physicsComponent = physicsSystem->physicsComponent;
	MovementComponent* movementComponent = physicsSystem->movementComponent;
	RectangleComponent* rectangleComponent = physicsSystem->rectangleComponent;
	HealthComponent* healthComponent = physicsSystem->healthComponent;
	HatComponent* hatComponent = physicsSystem->hatComponent;
	InputComponent* inputComponent = physicsSystem->inputComponent;
	GoalComponent* goalComponent = physicsSystem->goalComponent;
	InteractableComponent * interactableComponent = physicsSystem->interactableComponent;
	AliveComponent * aliveComponent = physicsSystem->aliveComponent;
	TileMap* map = physicsSystem->map;

	for (uint32 entityIndex = 0; entityIndex < physicsComponent->count; entityIndex++) {
		uint32 eid = physicsComponent->entityArray[entityIndex];
		if (!Component_HasIndex(movementComponent, eid)) {
			continue;
		}
		if (!Component_HasIndex(rectangleComponent, eid)) {
			continue;
		}
		MovementValues* moveValues = &movementComponent->movementValues[eid];
		Rectangle* r1 = &rectangleComponent->entityRectangles[eid];

		const Rectangle left 	= {r1->x, r1->y, 1, r1->h};
		const Rectangle right 	= {r1->x + r1->w, r1->y, 1, r1->h};
		const Rectangle up 		= {r1->x, r1->y, r1->w, 1};
		const Rectangle down 	= {r1->x+12, r1->y + r1->h, r1->w-13, 1};

		// Check collisions with entities
		for (uint32 j = 0; j < physicsComponent->count; j++) {
			uint32 otherEid = physicsComponent->entityArray[j];
			if (eid ==  otherEid) {
				continue;
			}
			if (!Component_HasIndex(rectangleComponent, otherEid)) {
				continue;
			}

			// Interaction collisions
			if (Component_HasIndex(interactableComponent, otherEid)) {
				if (!Collision(*r1, rectangleComponent->entityRectangles[otherEid])) {
					continue;
				}
				if (Component_HasIndex(inputComponent, eid)) {
			  		if(!inputComponent->interact[eid]) {
			  			continue;
			  		}
			  	}

				int type = interactableComponent->types[otherEid];
				if (type == InteractionType_Hat) {
					int hattype = interactableComponent->hattypes[otherEid];
					if (Component_HasIndex(hatComponent, eid)){
						if (!interactableComponent->interacted[otherEid]) {
							ApplyHatInteraction(hattype, eid, physicsSystem->componentBag);
							interactableComponent->interacted[otherEid] = true;
							if (Component_HasIndex(aliveComponent, otherEid)) {
						  		aliveComponent->alive[otherEid] = false;
						  	}
						}
					}
				}
				continue;
			}

			// Enemy collisions
			Rectangle r2 = rectangleComponent->entityRectangles[physicsComponent->entityArray[j]];
			bool cllsn = false;
			bool cllsnD = false;
			if (Collision(left, r2)) {
				r1->x -= (moveValues->xVelocity);
				cllsn = true;

				if (eid == Constants::PlayerIndex_) {
				  moveValues->xVelocity = 15;
				  if (!Collision(down, r2)) {
				      moveValues->yVelocity = -5;
				      r1->y += moveValues->yVelocity;
				  } else {
				    cllsnD = true;
				  }
				  r1->x += moveValues->xVelocity;
				}
			} else if (Collision(right, r2)) {
				r1->x -= (moveValues->xVelocity);
				cllsn = true;
				//kickback for player
				if (eid == Constants::PlayerIndex_) {
				  moveValues->xVelocity = -15;
				  r1->x += moveValues->xVelocity;
				  if (!Collision(down, r2)) {
				    moveValues->yVelocity = -5;
				    r1->y += moveValues->yVelocity;
				  } else {
				    cllsnD = true;
				  }
				}
			}
			if (Collision(up, r2)) {
			  moveValues->yVelocity = 0;
				cllsn = true;
			} else if (cllsnD || Collision(down, r2)) {
		        moveValues->yVelocity *= -1;
		        r1->y += moveValues->yVelocity;
				cllsn = true;
			}
			if (cllsn) {
			  if (Component_HasIndex(healthComponent, eid)) {
					if (!healthComponent->invincible[eid]) {
					  healthComponent->health[eid] -= Constants::Damage_ / healthComponent->damageReduction[eid];
					}
				}
			}
		}

		if (!moveValues->grounded) {
		  moveValues->yVelocity += Constants::Gravity_;
		  moveValues->xVelocity -= moveValues->xVelocity*Constants::AirRes_;
		} else {
		  moveValues->xVelocity -= moveValues->xVelocity*Constants::Friction_;
		}
		if (moveValues->xVelocity < 0.2 && moveValues->xVelocity > -0.2) {
		  moveValues->xVelocity = 0;
		}

		// Tiilemap collisions
		{
			int tileX = floor(r1->x / Constants::TileSize_);
			int tileCenterX = ((r1->x + (r1->w / 2)) / Constants::TileSize_);
			int tileEndX = floor((r1->x + r1->w) / Constants::TileSize_);
			int tileY = floor((r1->y + moveValues->yVelocity) / Constants::TileSize_);
			int tileCenterY = ((r1->y + (r1->h / 2)) / Constants::TileSize_);
			int tileEndY = floor((r1->y + r1->h) / Constants::TileSize_);
			int tileHeadY = floor((r1->y + 4) / Constants::TileSize_);
			int tileFootX = floor((r1->x + 12) / Constants::TileSize_);
			int tileEndFootX = floor((r1->x + r1->w - 12) / Constants::TileSize_);
			int tileHeadX = floor((r1->x + 4) / Constants::TileSize_);
			int tileEndHeadX = floor((r1->x + r1->w - 4) / Constants::TileSize_);
			
			moveValues->grounded = false;
			if (moveValues->yVelocity < 0) {
				if (map->map[tileY][tileHeadX].solid || map->map[tileY][tileEndHeadX].solid) {
					r1->y = tileY * Constants::TileSize_ + Constants::TileSize_;
					moveValues->yVelocity = 0;
				} 
			} else if (moveValues->yVelocity >= 0) {
				if (map->map[tileEndY][tileFootX].solid || map->map[tileEndY][tileEndFootX].solid) {
					r1->y = tileEndY * Constants::TileSize_ - r1->h;
					moveValues->yVelocity = 0;
					moveValues->grounded = true;
				}
			}

			if (moveValues->xVelocity < 0) {
				if (moveValues->grounded) {
					if (map->map[tileHeadY][tileX].solid || map->map[tileCenterY][tileX].solid) {
						r1->x = tileX * Constants::TileSize_ + Constants::TileSize_;
						moveValues->xVelocity = 0;
					}
				} else {
					if (map->map[tileHeadY][tileX].solid || map->map[tileCenterY][tileX].solid || map->map[tileEndY][tileX].solid) {
						r1->x = tileX * Constants::TileSize_ + Constants::TileSize_;
						moveValues->xVelocity = 0;
					}
				}
			} else if (moveValues->xVelocity > 0) {
				if (moveValues->grounded) {
					if (map->map[tileHeadY][tileEndX].solid || map->map[tileCenterY][tileEndX].solid) {
						r1->x = tileEndX * Constants::TileSize_ - r1->w;
						moveValues->xVelocity = 0;
					}
				} else {
					if (map->map[tileHeadY][tileEndX].solid || map->map[tileCenterY][tileEndX].solid || map->map[tileEndY][tileEndX].solid) {
						r1->x = tileEndX * Constants::TileSize_ - r1->w;
						moveValues->xVelocity = 0;
					}
				}
			}
		}

		// World boundary collisions
		if (r1->x <= 0) {
			r1->x = 0;
			moveValues->xVelocity = 0;
		} else if (r1->x + r1->w >= Constants::LevelWidth_) {
			r1->x = Constants::LevelWidth_ - r1->w;
			moveValues->xVelocity = 0;
		}
		if (r1->y < 0) {
			r1->y = 0;
			moveValues->yVelocity = 0;
		}
	}
}
