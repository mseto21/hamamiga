#include "PhysicsSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "PhysicsComponent.h"
#include "RectangleComponent.h"
#include "HealthComponent.h"
#include "BulletComponent.h"
#include "InputComponent.h"
#include "AIComponent.h"
#include "DamageComponent.h"
#include "GoalComponent.h"
#include "InteractableComponent.h"
#include "TileMap.h"
#include "ComponentBag.h"
#include "SoundCache.h"
#include "Interactions.h"
#include "Texture.h"
#include "Zone.h"

#include <stdlib.h>
#include <SDL.h>
#include <iostream>

// Physics Constants
void PhysicsSystem_Initialize(PhysicsSystem* physicsSystem, ComponentBag* cBag, TileMap* tileMap, Zone* zone) {
	physicsSystem->physicsComponent 	= cBag->physicsComponent;
	physicsSystem->movementComponent 	= cBag->movementComponent;
	physicsSystem->rectangleComponent 	= cBag->rectangleComponent;
	physicsSystem->healthComponent 		= cBag->healthComponent;
	physicsSystem->bulletComponent  	= cBag->bulletComponent;
	physicsSystem->inputComponent		= cBag->inputComponent;
	physicsSystem->interactableComponent = cBag->interactableComponent;
	physicsSystem->aiComponent          = cBag->aiComponent;
	physicsSystem->damageComponent      = cBag->damageComponent;
	physicsSystem->map 					= tileMap;
	physicsSystem->componentBag 		= cBag;
	physicsSystem->zone 				= zone;
}


static bool Collision(const Rectangle r1, const Rectangle r2) {
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
	BulletComponent* bulletComponent = physicsSystem->bulletComponent;
	InteractableComponent * interactableComponent = physicsSystem->interactableComponent;
	AIComponent * aiComponent = physicsSystem->aiComponent;
	DamageComponent * damageComponent = physicsSystem->damageComponent;
	TileMap* map = physicsSystem->map;

	for (uint32 entityIndex = 0; entityIndex < physicsComponent->count; entityIndex++) {
		uint32 eid = physicsComponent->entityArray[entityIndex];
		// Component checks are bad...
		if (!Component_HasIndex(physicsComponent, eid)) {
			continue;
		}
		if (!Component_HasIndex(movementComponent, eid)) {
			continue;
		}
		if (!Component_HasIndex(rectangleComponent, eid)) {
			continue;
		}
		
		MovementValues* moveValues = &movementComponent->movementValues[eid];
		Rectangle* r1 = &rectangleComponent->entityRectangles[eid];
		if (Component_HasIndex(interactableComponent, eid)) {
			goto world_physics;
		}

		// Check collisions with entities
		for (uint32 j = 0; j < physicsComponent->count; j++) {
			uint32 otherEid = physicsComponent->entityArray[j];
			if (eid == otherEid) {
				continue;
			}
			if (!Component_HasIndex(rectangleComponent, otherEid)) {
				continue;
			}
			if (Component_HasIndex(interactableComponent, otherEid)) {
				continue;
			}
			if (Component_HasIndex(healthComponent, eid) && healthComponent->invincible[eid] > 0) {
			        healthComponent->invincible[eid]--;
			        continue;
			}
			if (Component_HasIndex(healthComponent, eid) && healthComponent->invincible[otherEid] > 0) {
			        continue;
			}

			//Don't collide with bullets on your team
			if (Component_HasIndex(bulletComponent, otherEid)){
				if (eid == Constants::PlayerIndex_ && bulletComponent->bullet[otherEid].friendly == true){
					continue;//player cannot collide with own bullets
				}
				if (eid != Constants::PlayerIndex_ && bulletComponent->bullet[otherEid].friendly != true){
					continue;//enemy cannot collide with own bullets
				}
				if (Component_HasIndex(bulletComponent, eid)){
					continue;//bullets cannot collide with other bullets
				}
			}

			// Reverse if entity is bullet and otherEid is player, enemy
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

			// Enemy collisions
			Rectangle r2 = rectangleComponent->entityRectangles[physicsComponent->entityArray[j]];
			if (Collision(*r1, r2)) {
				if (!Component_HasIndex(aiComponent, eid)) {
					int xVelocity = 0;
					int yVelocity = 0;

					int collisionXVelocity = movementComponent->movementValues[otherEid].xVelocity;
					float* collisionYVelocity = &movementComponent->movementValues[otherEid].yVelocity;

					if (*collisionYVelocity > 0) {
						*collisionYVelocity = -*collisionYVelocity;
					}

					if (collisionXVelocity != 0)
						xVelocity = 2 * collisionXVelocity;
					else
						xVelocity = -2 * moveValues->xVelocity;
					moveValues->xVelocity = xVelocity;
					r1->x += moveValues->xVelocity;

					if (moveValues->yVelocity > 0)
						yVelocity = -10;
					moveValues->yVelocity = yVelocity;
					r1->y += moveValues->yVelocity;
				}
				
				if (Component_HasIndex(healthComponent, eid)) {
				  if (healthComponent->invincible[eid] <= 0) {
				    healthComponent->startHealth[eid] = healthComponent->health[eid];
				    if (Component_HasIndex(damageComponent, otherEid)) {
				    	if (!(Component_HasIndex(aiComponent, eid) && Component_HasIndex(aiComponent, otherEid))) {
							if (!((Component_HasIndex(aiComponent, eid) || Component_HasIndex(interactableComponent, eid))
					      		&& (Component_HasIndex(aiComponent, otherEid) || Component_HasIndex(interactableComponent, otherEid)))) {
					  			healthComponent->health[eid] -= damageComponent->damageValues[otherEid].damage / healthComponent->damageReduction[eid];
					  			if (eid == Constants::PlayerIndex_) {
					    			healthComponent->invincible[eid] = 640;
					  			}
							}
				      	} else {
							aiComponent->marchValues[eid].facing *= -1;
							aiComponent->marchValues[eid].distance = 0;
							movementComponent->movementValues[eid].xAccel *= -1;
							r1->x += movementComponent->movementValues[eid].xAccel*10;
				      	}
				    }
				  }
				  bulletComponent->bullet[otherEid].collided = true;
				}
			}
		}

world_physics:
		// Move player based on physics
		if (!moveValues->grounded) {
			if (moveValues->flying) {
			    moveValues->yVelocity += Constants::Gravity_ / 2;
			} else {
				moveValues->yVelocity += Constants::Gravity_;
			}
		  	moveValues->xVelocity -= moveValues->xVelocity*Constants::AirRes_;
		  	moveValues->xAccel -= moveValues->xAccel*Constants::AirRes_;
			
		} else {
			moveValues->xAccel -= moveValues->xAccel*Constants::Friction_;
		 	moveValues->xVelocity -= moveValues->xVelocity*Constants::Friction_;
		}
		if (moveValues->xVelocity < 0.2 && moveValues->xVelocity > -0.2) {
			moveValues->xVelocity = 0;
		}

		// Tiilemap collisions
		{
			//don't collide with tiles if bullet
			int tileX = floor(r1->x / Constants::TileSize_);
			int tileEndX = floor((r1->x + r1->w) / Constants::TileSize_);
			int tileY = floor((r1->y + moveValues->yVelocity) / Constants::TileSize_);
			int tileCenterY = ((r1->y + (r1->h / 2)) / Constants::TileSize_);
			int tileEndY = floor((r1->y + r1->h) / Constants::TileSize_);
			int tileHeadY = floor((r1->y + 4) / Constants::TileSize_);
			int tileFootX = floor((r1->x + 12) / Constants::TileSize_);
			int tileEndFootX = floor((r1->x + r1->w - 12) / Constants::TileSize_);
			int tileHeadX = floor((r1->x + 4) / Constants::TileSize_);
			int tileEndHeadX = floor((r1->x + r1->w - 4) / Constants::TileSize_);

			if (tileEndY * Constants::TileSize_ > map->w * Constants::TileSize_) {
				continue;
			}

			moveValues->grounded = false;
			if (moveValues->yVelocity < 0) {
				if (map->map[tileY][tileHeadX].solid || map->map[tileY][tileEndHeadX].solid) {
					r1->y = tileY * Constants::TileSize_ + Constants::TileSize_;
					moveValues->yVelocity = 0;
					if (Component_HasIndex(bulletComponent, eid)){
							bulletComponent->bullet[eid].collided = true;
					}
				} 
			} else if (moveValues->yVelocity >= 0) {
				if (map->map[tileEndY][tileFootX].solid || map->map[tileEndY][tileEndFootX].solid) {
					r1->y = tileEndY * Constants::TileSize_ - r1->h;
					moveValues->yVelocity = 0;
					moveValues->grounded = true;
					if (Component_HasIndex(bulletComponent, eid)){
							bulletComponent->bullet[eid].collided = true;
					}
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
						if (Component_HasIndex(aiComponent, eid)) {
						  aiComponent->marchValues[eid].facing *= -1;
						  aiComponent->marchValues[eid].distance = 0;
						}
					}
					if (Component_HasIndex(aiComponent, eid) && !map->map[tileEndY][tileX].solid) {
					  aiComponent->marchValues[eid].facing *= -1;
					  aiComponent->marchValues[eid].distance = 0;
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
						if (Component_HasIndex(aiComponent, eid)) {
						  aiComponent->marchValues[eid].facing *= -1;
						  aiComponent->marchValues[eid].distance = 0;
						}
					}
     					if (Component_HasIndex(aiComponent, eid) && !map->map[tileEndY][tileEndX].solid) {
					  aiComponent->marchValues[eid].facing *= -1;
					  aiComponent->marchValues[eid].distance = 0;
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
			} else if (r1->x + r1->w >= physicsSystem->zone->levelWidth) {
				r1->x = physicsSystem->zone->levelWidth - r1->w;
				moveValues->xVelocity = 0;
			}
			if (r1->y < 0) {
				r1->y = 0;
				moveValues->yVelocity = 0;
			}
		}
		//if (eid == Constants::PlayerIndex_) {
		  //std::cout << "x: " << r1->x << " y: " << r1->y << std::endl;
		  //}
	}
}

void PhysicsSystem_Free(PhysicsSystem* physicsSystem) {
	physicsSystem->physicsComponent = nullptr;
	physicsSystem->movementComponent = nullptr;
	physicsSystem->rectangleComponent = nullptr;
	physicsSystem->healthComponent = nullptr;
	physicsSystem->bulletComponent = nullptr;
	physicsSystem->inputComponent = nullptr;
	physicsSystem->interactableComponent = nullptr;
	physicsSystem->map = nullptr;
	physicsSystem->componentBag = nullptr;
}
