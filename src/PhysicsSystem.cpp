#include "PhysicsSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "PhysicsComponent.h"
#include "RectangleComponent.h"
#include "HealthComponent.h"
#include "HatComponent.h"
#include "TileMap.h"

#include <SDL.h>
#include <iostream>

bool collision(const Rectangle* r1, const Rectangle* r2);

bool PhysicsSystem_Update(float timestep, PhysicsComponent* physicsComponent, MovementComponent* movementComponent, RectangleComponent* rectangleComponent, 
	HealthComponent* healthComponent, HatComponent* hatComponent, TileMap* map) {

	for (uint32 entityIndex = 0; entityIndex < physicsComponent->count; entityIndex++) {
		if (!Component_HasIndex(movementComponent, physicsComponent->entityArray[entityIndex])) {
			continue;
		}
		// Get the movement value
		MovementValues* moveValues = &movementComponent->movementValues[movementComponent->entityArray[entityIndex]];
		

		// Check collisions with entities
		if (!Component_HasIndex(rectangleComponent, physicsComponent->entityArray[entityIndex])) {
			continue;
		}
		Rectangle* r1 = &rectangleComponent->entityRectangles[rectangleComponent->entityArray[entityIndex]];
		for (uint32 j = (entityIndex+1); j < physicsComponent->count; j++) {
			if (!Component_HasIndex(rectangleComponent, physicsComponent->entityArray[j])) {
				continue;
			}

			Rectangle* r2 = &rectangleComponent->entityRectangles[rectangleComponent->entityArray[j]];
			if (collision(r1, r2)) {
				r1->x -= moveValues->xVelocity;
				r1->y -= moveValues->yVelocity;
				moveValues->xVelocity *= -1;
				moveValues->yVelocity *= -1;

				// Get damage if necessary
				if (Component_HasIndex(healthComponent, physicsComponent->entityArray[entityIndex])) {
					int dmgRed = 1;
					if (Component_HasIndex(hatComponent, physicsComponent->entityArray[entityIndex])) {
						Hat* hat = &hatComponent->hats[hatComponent->entityArray[entityIndex]].hat;
						dmgRed = hat->getDmgRed();
					}
					healthComponent->health[entityIndex] -= Constants::Damage_/dmgRed;
				}
		      
				MovementValues* moveValues2 = &movementComponent->movementValues[movementComponent->entityArray[j]];
				r2->x -= moveValues2->xVelocity;
				r2->y -= moveValues->yVelocity;
				moveValues2->xVelocity *= -1;
				moveValues2->yVelocity *= -1;
		      
				if (Component_HasIndex(healthComponent, physicsComponent->entityArray[j])) {
					int dmgRed = 1;
					if (Component_HasIndex(hatComponent, physicsComponent->entityArray[j])) {
						Hat* hat = &hatComponent->hats[hatComponent->entityArray[entityIndex]].hat;
						dmgRed = hat->getDmgRed();
					}
					healthComponent->health[j] -= Constants::Damage_/dmgRed;
				}
			}
		}

		moveValues->xVelocity -= Constants::Friction_ * moveValues->xVelocity;

		// TO-DO: Make this generic for all maps.
		int tileX = floor(r1->x / Constants::TileSize_);
		int tileY = floor(r1->y / Constants::TileSize_);
		int tileEndX = floor((r1->x + r1->w) / Constants::TileSize_);
		int tileEndY = floor((r1->y + r1->h) / Constants::TileSize_);
		int tileCenterX = floor((r1->x + (r1->w / 2)) / Constants::TileSize_);
		int tileCenterY = floor((r1->y + (r1->h / 2)) / Constants::TileSize_);
		moveValues->yVelocity += Constants::Gravity_ * timestep;

		// Check down collisions only if we're not jumping
		if (moveValues->yVelocity > 0) {
			if (map->map[tileEndY][tileX].solid || map->map[tileEndY][tileEndX].solid) {
				r1->y = tileEndY * Constants::TileSize_ - r1->h;
				moveValues->yVelocity = 0;
			}
		}

		if (moveValues->xVelocity != 0) {
			if (map->map[tileCenterY][tileX].solid && moveValues->xVelocity < 0) {
				moveValues->xVelocity = 0;
			} else if (map->map[tileCenterY][tileEndX].solid && moveValues->xVelocity > 0) {
				moveValues->xVelocity = 0;
			}
		}

		if (physicsComponent->entityArray[entityIndex] == 0 && map->map[tileCenterY][tileCenterX].winning) {
			return true;
		}

	}
	return false;
}

bool collision(const Rectangle* r1, const Rectangle* r2) {
  if (r1->x <= r2->x + r2->w && r1->x + r1->w >= r2->x && r1->y <= r2->y + r2->h && r1->y + r1->h >= r2->y) {
    return true;
  }
  return false;
}
