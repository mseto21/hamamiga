#include "PhysicsSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "PhysicsComponent.h"
#include "RectangleComponent.h"
#include "HealthComponent.h"
#include "HatComponent.h"

#include <SDL.h>
#include <iostream>

bool collision(const Rectangle* r1, const Rectangle* r2);

void PhysicsSystem_Update(float timestep, PhysicsComponent* physicsComponent, MovementComponent* movementComponent, RectangleComponent* rectangleComponent, HealthComponent* healthComponent, HatComponent* hatComponent) {
	for (uint32 entityIndex = 0; entityIndex < physicsComponent->count; entityIndex++) {
		if (!Component_HasIndex(movementComponent, physicsComponent->entityArray[entityIndex])) {
			continue;
		}
		// Get the movement value
		MovementValues* moveValues = &movementComponent->movementValues[movementComponent->entityArray[entityIndex]];
		

		//Collisions
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
		      r1->x -= moveValues->xVelocity * timestep;
		      r1->y -= moveValues->yVelocity * timestep;
		      moveValues->xVelocity *= -1;
		      moveValues->yVelocity *= -1;
		      if (Component_HasIndex(healthComponent, physicsComponent->entityArray[entityIndex])) {
						int dmgRed = 1;
						if (Component_HasIndex(hatComponent, physicsComponent->entityArray[entityIndex])) {
						  Hat* hat = &hatComponent->hats[hatComponent->entityArray[entityIndex]].hat;
						  dmgRed = hat->getDmgRed();
						}
			  		healthComponent->health[entityIndex] -= Constants::Damage_/dmgRed;
		      }
		      
		      MovementValues* moveValues2 = &movementComponent->movementValues[movementComponent->entityArray[j]];
		      r2->x -= moveValues2->xVelocity * timestep;
		      r2->y -= moveValues->yVelocity * timestep;
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
		moveValues->yVelocity += Constants::Gravity_*timestep; //gravity
		moveValues->xVelocity -= Constants::Friction_*moveValues->xVelocity*timestep;
	}
}

bool collision(const Rectangle* r1, const Rectangle* r2) {
  if (r1->x <= r2->x + r2->w && r1->x + r1->w >= r2->x && r1->y <= r2->y + r2->h && r1->y + r1->h >= r2->y) {
    return true;
  }
  return false;
}
