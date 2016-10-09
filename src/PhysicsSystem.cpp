#include "PhysicsSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "PhysicsComponent.h"
#include "RectangleComponent.h"
#include "HealthComponent.h"

#include <SDL.h>
#include <iostream>

void PhysicsSystem_Update(PhysicsComponent* physicsComponent, MovementComponent* movementComponent, RectangleComponent* rectangleComponent, HealthComponent* healthComponent) {
	for (uint32 entityIndex = 0; entityIndex < physicsComponent->count; entityIndex++) {
		if (!Component_HasIndex(movementComponent, entityIndex)) {
			continue;
		}
		// Get the movement value
		MovementValues* moveValues = &movementComponent->movementValues[movementComponent->entityArray[entityIndex]];
		moveValues->yVelocity += Constants::Gravity_; //gravity

		//Collisions
		Rectangle* r1 = &rectangleComponent->entityRectangles[rectangleComponent->entityArray[entityIndex]];/*
		for (uint32 j = (entityIndex+1); j < physicsComponent->count; j++) {
		  Rectangle* r2 = &rectangleComponent->entityRectangles[rectangleComponent->entityArray[j]];
		  if ((r1->x >= r2->x && (r1->x <= (r2->x + r2->w)))
		      || ((r1->x + r1->w) >= r2->x && (r1->x + r1->w) <= (r2->x + r2->w))) {
		    if ((r1->y >= r2->y && (r1->y <= (r2->y + r2->h)))
			|| ((r1->y = r2->h) >= r2->y && (r2->y + r2->h) <= (r2->y +r2->h))) {
		      //r1->x -= moveValues->xVelocity;
		      //r1->y -= (moveValues->yVelocity - Constants::Gravity_);
		      std::cout << "collision j: " << j << " eid: " << entityIndex << std::endl;
		      if (Component_HasIndex(healthComponent, entityIndex)) {
			//r1->x -= moveValues->xVelocity;
			//r1->y -= moveValues->yVelocity;
			  healthComponent->health[entityIndex] -= Constants::Damage_;
		      } else {
			moveValues->xVelocity *= -1;
		      }
		      
		      MovementValues* moveValues2 = &movementComponent->movementValues[movementComponent->entityArray[j]];
		      if (Component_HasIndex(healthComponent, j)) {
			  healthComponent->health[j] -= Constants::Damage_;
		      } else {
			moveValues2->xVelocity *= -1;
		      }
		    }
		  }
		  }*/
		  
		}
}
