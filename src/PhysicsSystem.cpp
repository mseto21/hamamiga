#include "PhysicsSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "PhysicsComponent.h"
#include "RectangleComponent.h"
#include "HealthComponent.h"
#include "BulletComponent.h"
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

// Physics Constants
void PhysicsSystem_Initialize(PhysicsSystem* physicsSystem, ComponentBag* cBag, TileMap* tileMap) {
	physicsSystem->physicsComponent 	= cBag->physicsComponent;
	physicsSystem->movementComponent 	= cBag->movementComponent;
	physicsSystem->rectangleComponent 	= cBag->rectangleComponent;
	physicsSystem->healthComponent 		= cBag->healthComponent;
	physicsSystem->bulletComponent  	= cBag->bulletComponent;
	physicsSystem->hatComponent 		= cBag->hatComponent;
	physicsSystem->inputComponent		= cBag->inputComponent;
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
	BulletComponent* bulletComponent = physicsSystem->bulletComponent;
	InputComponent* inputComponent = physicsSystem->inputComponent;
	InteractableComponent * interactableComponent = physicsSystem->interactableComponent;
	AliveComponent * aliveComponent = physicsSystem->aliveComponent;
	TileMap* map = physicsSystem->map;

	for (uint32 entityIndex = 0; entityIndex < physicsComponent->count; entityIndex++) {
		uint32 eid = physicsComponent->entityArray[entityIndex];

		// Component checks are bad.... FUCK PHYSICS.
		if (!Component_HasIndex(physicsComponent, eid)) {
			continue;
		}
		if (!Component_HasIndex(movementComponent, eid)) {
			continue;
		}
		if (!Component_HasIndex(rectangleComponent, eid)) {
			continue;
		}
		if (Component_HasIndex(interactableComponent, eid)) {
			continue;
		}


		MovementValues* moveValues = &movementComponent->movementValues[eid];
		Rectangle* r1 = &rectangleComponent->entityRectangles[eid];

		// Check collisions with entities
		const Rectangle left 	= {r1->x, r1->y, 1, r1->h};
		const Rectangle right 	= {r1->x + r1->w, r1->y, 1, r1->h};
		const Rectangle up 		= {r1->x, r1->y, r1->w, 1};
		const Rectangle down 	= {r1->x+12, r1->y + r1->h, r1->w-13, 1};
		for (uint32 j = 0; j < physicsComponent->count; j++) {
			uint32 otherEid = physicsComponent->entityArray[j];
			if (eid ==  otherEid) {
				continue;
			}
			if (!Component_HasIndex(rectangleComponent, otherEid)) {
				continue;
			}
			//Don't collide with bullets on your team
			if (Component_HasIndex(bulletComponent, otherEid)){
				if (eid == Constants::PlayerIndex_&&
					bulletComponent->bullet[otherEid].friendly == true){
					continue;//player cannot collide with own bullets
				}
				if (eid != Constants::PlayerIndex_&& 
					bulletComponent->bullet[otherEid].friendly != true){
					continue;//enemy cannot collide with own bullets
				}
				if (Component_HasIndex(bulletComponent, eid)){
					continue;//bullets cannot collide with other bullets
				}
			}
			//Reverse if entity is bullet and otherEid is player, enemy
			if (Component_HasIndex(bulletComponent, eid)){
				if (otherEid == Constants::PlayerIndex_&&
					bulletComponent->bullet[eid].friendly == true){
					continue;//player cannot collide with own bullets
				}
				if (otherEid != Constants::PlayerIndex_&& 
					bulletComponent->bullet[eid].friendly != true){
					continue;//enemy cannot collide with own bullets
				}
			}
			

			// Interaction collisions
			if (Component_HasIndex(interactableComponent, otherEid)) {
 				if (!Collision(*r1, rectangleComponent->entityRectangles[otherEid])) {
 					if (eid == Constants::PlayerIndex_)
 						interactableComponent->canBeInteractedWith[otherEid] = false;
 					continue;
 				}

 				if (eid == Constants::PlayerIndex_)
 					interactableComponent->canBeInteractedWith[otherEid] = true;
 				
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
					continue;
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

			//if bullet has collided, kill it
			if (cllsn && Component_HasIndex(bulletComponent, physicsComponent->entityArray[j])){
				bulletComponent->bullet[physicsComponent->entityArray[j]].collided = true;
			}
			if (cllsn) {
				if (Component_HasIndex(healthComponent, eid)) {
					if (!healthComponent->invincible[eid]) {
						healthComponent->health[eid] -= Constants::Damage_ / healthComponent->damageReduction[eid];
						if (Component_HasIndex(aliveComponent, eid) && healthComponent->health[eid] <= 0){
							aliveComponent->alive[eid] = false;
						}
					}
				}
			}
		}

		// Move player based on physics
		if (!moveValues->grounded && !moveValues->flying) {
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
			//don't collide with tiles if bullet
				int tileX = floor(r1->x / Constants::TileSize_);
				//int tileCenterX = ((r1->x + (r1->w / 2)) / Constants::TileSize_);
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
						if (map->map[tileEndY][tileFootX].speed > 0) {
							moveValues->xVelocity += map->map[tileEndY][tileFootX].speed;
						} else if (map->map[tileEndY][tileEndFootX].speed > 0) {
							moveValues->xVelocity += map->map[tileEndY][tileEndFootX].speed;
						}
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
							if (Component_HasIndex(bulletComponent, eid)){
								bulletComponent->bullet[eid].collided = true;
							}
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
							if (Component_HasIndex(bulletComponent, eid)){
								bulletComponent->bullet[eid].collided = true;
							}
						}
					}
				}
		}

		// World boundary collisions
		if (!Component_HasIndex(bulletComponent, eid)){
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
}

void PhysicsSystem_Free(PhysicsSystem* physicsSystem) {
	physicsSystem->physicsComponent = nullptr;
	physicsSystem->movementComponent = nullptr;
	physicsSystem->rectangleComponent = nullptr;
	physicsSystem->healthComponent = nullptr;
	physicsSystem->hatComponent = nullptr;
	physicsSystem->bulletComponent = nullptr;
	physicsSystem->inputComponent = nullptr;
	physicsSystem->interactableComponent = nullptr;
	physicsSystem->aliveComponent = nullptr;
	physicsSystem->map = nullptr;
	physicsSystem->componentBag = nullptr;
}
