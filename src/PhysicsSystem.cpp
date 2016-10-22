#include "PhysicsSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "PhysicsComponent.h"
#include "RectangleComponent.h"
#include "HealthComponent.h"
#include "HatComponent.h"
#include "TileMap.h"
#include "Hat.h"
#include "ComponentBag.h"

#include <SDL.h>
#include <iostream>

void PhysicsSystem_Initialize(PhysicsSystem* physicsSystem, ComponentBag* cBag, TileMap* tileMap) {
	physicsSystem->physicsComponent 	= cBag->physicsComponent;
	physicsSystem->movementComponent 	= cBag->movementComponent;
	physicsSystem->rectangleComponent  	= cBag->rectangleComponent;
	physicsSystem->healthComponent 		= cBag->healthComponent;
	physicsSystem->hatComponent 		= cBag->hatComponent;
	physicsSystem->map 					= tileMap;
}


bool Collision(const Rectangle r1, const Rectangle r2) {
  return r1.x <= r2.x + r2.w 
  		&& r1.x + r1.w >= r2.x 
  		&& r1.y <= r2.y + r2.h 
  		&& r1.y + r1.h >= r2.y;
}


int PhysicsSystem_Update(PhysicsSystem* physicsSystem) {
	PhysicsComponent* physicsComponent = physicsSystem->physicsComponent;
	MovementComponent* movementComponent = physicsSystem->movementComponent;
	RectangleComponent* rectangleComponent = physicsSystem->rectangleComponent;
	HealthComponent* healthComponent = physicsSystem->healthComponent;
	HatComponent* hatComponent = physicsSystem->hatComponent;
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
		const Rectangle down 	= {r1->x+12, r1->y + r1->h, r1->w-25, 1};

		// Check collisions with entities
		for (uint32 j = 0; j < physicsComponent->count; j++) {
			if (eid ==  physicsComponent->entityArray[j]) {
				continue;
			}
			if (!Component_HasIndex(rectangleComponent, physicsComponent->entityArray[j])) {
				continue;
			}

			Rectangle r2 = rectangleComponent->entityRectangles[physicsComponent->entityArray[j]];
			bool cllsn = false;
			if (Collision(left, r2)) {
				r1->x -= (moveValues->xVelocity);
				cllsn = true;
			} else if (Collision(right, r2)) {
				r1->x -= (moveValues->xVelocity);
				cllsn = true;
			}
			if (Collision(up, r2)) {
			        moveValues->yVelocity = 0;
				cllsn = true;
			} else if (Collision(down, r2)) {
				moveValues->yVelocity *= -1;
				cllsn = true;
			}
			if (cllsn) {
			  if (Component_HasIndex(healthComponent, eid)) {
					int dmgRed = 1;
					if (Component_HasIndex(hatComponent, eid)) {
						Hat* hat = &hatComponent->hats[eid].hat;
						dmgRed = hat->getDmgRed();
					}
					std::cout << healthComponent->health[eid] << " :health" << std::endl;
					healthComponent->health[eid] -= Constants::Damage_/dmgRed;
					if (healthComponent->health[eid] <= 0) {
					  return -1;
					}
				}
			}
		}

		if (!moveValues->grounded) {
		  moveValues->yVelocity += Constants::Gravity_;
		}
		if (moveValues->xVelocity != 0) {
			if (moveValues->xVelocity > 0) {
				moveValues->xVelocity -= Constants::Friction_;
				if (moveValues->xVelocity < 0) {
					moveValues->xVelocity = 0;
				}
			}
			else {
				moveValues->xVelocity += Constants::Friction_;
				if (moveValues->xVelocity > 0) {
					moveValues->xVelocity = 0;
				}
			}
		}

		// Check for collisions with map
		{
			int tileX = floor(r1->x / Constants::TileSize_);
			int tileCenterX = ((r1->x + (r1->w / 2)) / Constants::TileSize_);
			int tileEndX = floor((r1->x + r1->w) / Constants::TileSize_);
			int tileY = floor((r1->y) / Constants::TileSize_);
			int tileCenterY = ((r1->y + (r1->h / 2)) / Constants::TileSize_);
			int tileEndY = floor((r1->y + r1->h) / Constants::TileSize_);

			moveValues->grounded = false;
			if (moveValues->yVelocity < 0) {
				if (map->map[tileY][tileX].solid || map->map[tileY][tileEndX].solid) {
					r1->y = tileY * Constants::TileSize_ + Constants::TileSize_;
					moveValues->yVelocity = 0;
				} 
			} else if (moveValues->yVelocity >= 0) {
				if (map->map[tileEndY][tileX].solid || map->map[tileEndY][tileEndX].solid) {
					r1->y = tileEndY * Constants::TileSize_ - r1->h;
					moveValues->yVelocity = 0;
					moveValues->grounded = true;
				}
			}

			if (moveValues->xVelocity < 0) {
				if (moveValues->grounded) {
					if (map->map[tileY][tileX].solid || map->map[tileCenterY][tileX].solid) {
						r1->x = tileX * Constants::TileSize_ + Constants::TileSize_;
						moveValues->xVelocity = 0;
					}
				} else {
					if (map->map[tileY][tileX].solid || map->map[tileCenterY][tileX].solid || map->map[tileEndY][tileX].solid) {
						r1->x = tileX * Constants::TileSize_ + Constants::TileSize_;
						moveValues->xVelocity = 0;
					}
				}
			} else if (moveValues->xVelocity > 0) {
				if (moveValues->grounded) {
					if (map->map[tileY][tileEndX].solid || map->map[tileCenterY][tileEndX].solid) {
						r1->x = tileEndX * Constants::TileSize_ - r1->w;
						moveValues->xVelocity = 0;
					}
				} else {
					if (map->map[tileY][tileEndX].solid || map->map[tileCenterY][tileEndX].solid || map->map[tileEndY][tileX].solid) {
						r1->x = tileEndX * Constants::TileSize_ - r1->w;
						moveValues->xVelocity = 0;
					}
				}
			}

		if (map->map[tileCenterY][tileCenterX].bunny) {
			if (Component_HasIndex(hatComponent, eid)) {	    
				HatCollection* hats = &hatComponent->hats[hatComponent->entityArray[entityIndex]];
				hats->hat = Hat(0);
			}
		}

		// Check if the game is won
		if (physicsComponent->entityArray[entityIndex] == 0 && map->map[tileCenterY][tileCenterX].winning) {
			return 1;
		}
		//finish map collisions

		// Check world boundaries
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

		if (r1->y > Constants::LevelHeight_) {
		  //REMOVE ENTITY
		  if (eid == Constants::PlayerIndex_) {
		    return -1;
		  }
		}

		// Continue on static object
		if (moveValues->xVelocity == 0 || moveValues->yVelocity == 0) {
			continue;
		}

		}
	return 0;
	}
}
