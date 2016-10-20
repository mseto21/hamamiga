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
bool PhysicsSystem_Update(PhysicsSystem* physicsSystem, float timestep);


bool Collision(const Rectangle r1, const Rectangle r2) {
  return r1.x <= r2.x + r2.w 
  		&& r1.x + r1.w >= r2.x 
  		&& r1.y <= r2.y + r2.h 
  		&& r1.y + r1.h >= r2.y;
}


bool PhysicsSystem_Update(PhysicsSystem* physicsSystem, float timestep) {
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
		const Rectangle down 	= {r1->x, r1->y + r1->h, r1->w, 1};

		// Check collisions with entities
		for (uint32 j = 0; j < physicsComponent->count; j++) {
			if (eid ==  physicsComponent->entityArray[j]) {
				continue;
			}
			if (!Component_HasIndex(rectangleComponent, physicsComponent->entityArray[j])) {
				continue;
			}

			Rectangle r2 = rectangleComponent->entityRectangles[physicsComponent->entityArray[j]];

			if (Collision(left, r2)) {
				r1->x -= (moveValues->xVelocity);
			}
			if (Collision(right, r2)) {
				r1->x -= (moveValues->xVelocity);
			}
			if (Collision(up, r2)) {
				moveValues->yVelocity = 0;
			}
			if (Collision(down, r2)) {
				moveValues->yVelocity = -Constants::Gravity_*timestep;
			}
		}


		moveValues->yVelocity += Constants::Gravity_ * timestep;
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
		int tileX = floor(r1->x / Constants::TileSize_);
		int tileY = floor(r1->y / Constants::TileSize_);
		int tileEndX = floor((r1->x + r1->w) / Constants::TileSize_);
		int tileEndY = floor((r1->y + r1->h) / Constants::TileSize_);
		int tileCenterX = floor((r1->x + (r1->w / 2)) / Constants::TileSize_);
		int tileCenterY = floor((r1->y + (r1->h / 2)) / Constants::TileSize_);


		if (moveValues->yVelocity != 0) {
			if (map->map[tileY][tileX].solid || map->map[tileY][tileEndX].solid) {
				moveValues->yVelocity = Constants::Gravity_ * timestep;
				std::cout << "Up" << std::endl;
			}
			if (map->map[tileEndY][tileX].solid || map->map[tileEndY][tileEndX].solid) {
				r1->y = tileEndY * Constants::TileSize_ - r1->h;
				moveValues->yVelocity = 0;
				std::cout << "Down" << std::endl;
			}
		}

		if (moveValues->xVelocity != 0) {
			if (map->map[tileCenterY][tileX].solid || map->map[tileY][tileX].solid) {
				r1->x = tileX * Constants::TileSize_;
			}
			if (map->map[tileCenterY][tileEndX].solid || map->map[tileY][tileEndX].solid) {
				r1->x = tileEndX * Constants::TileSize_ - r1->w;
				std::cout << "Right" << std::endl;
			}
		}


		if (map->map[tileCenterY][tileCenterX].bunny) {
			if (Component_HasIndex(hatComponent, hatComponent->entityArray[entityIndex])) {	    
				HatCollection* hats = &hatComponent->hats[hatComponent->entityArray[entityIndex]];
				hats->hat = Hat(0);
			}
		}

		// Check if the game is won
		if (physicsComponent->entityArray[entityIndex] == 0 && map->map[tileCenterY][tileCenterX].winning) {
			return true;
		}


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

		// Continue on static object
		if (moveValues->xVelocity == 0 || moveValues->yVelocity == 0) {
			continue;
		}

	}
	return false;
}

/*
			if (Collision(r1, r2)) {
				r1->x -= (moveValues->xVelocity * timestep) * -1;
				r1->y -= (moveValues->yVelocity * timestep) * -1;

				// Get damage if necessary
				if (Component_HasIndex(healthComponent, physicsComponent->entityArray[entityIndex])) {
					int dmgRed = 1;
					if (Component_HasIndex(hatComponent, physicsComponent->entityArray[entityIndex])) {
						Hat* hat = &hatComponent->hats[hatComponent->entityArray[entityIndex]].hat;
						dmgRed = hat->getDmgRed();
					}
					healthComponent->health[entityIndex] -= Constants::Damage_/dmgRed;
				}
		      
		      	if (!Component_HasIndex(movementComponent, physicsComponent->entityArray[j])) {
					continue;
				}
				MovementValues* moveValues2 = &movementComponent->movementValues[movementComponent->entityArray[j]];
				r2->x -= (moveValues->xVelocity * timestep) * -1;
				r2->y -= (moveValues->yVelocity * timestep) * -1;
		      
				if (Component_HasIndex(healthComponent, physicsComponent->entityArray[j])) {
					int dmgRed = 1;
					if (Component_HasIndex(hatComponent, physicsComponent->entityArray[j])) {
						Hat* hat = &hatComponent->hats[hatComponent->entityArray[entityIndex]].hat;
						dmgRed = hat->getDmgRed();
					}
					healthComponent->health[j] -= Constants::Damage_/dmgRed;
				}
			}*/
