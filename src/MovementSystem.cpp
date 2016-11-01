#include "MovementSystem.h"
#include "MovementComponent.h"
#include "RectangleComponent.h"
#include "HatComponent.h"
#include "ComponentBag.h"

#include <iostream>

void MovementSystem_Initialize(MovementSystem* movementSystem, ComponentBag* cBag) {
	movementSystem->movementComponent 	=	cBag->movementComponent;
	movementSystem->rectangleComponent      =	cBag->rectangleComponent;
	movementSystem->hatComponent            =       cBag->hatComponent;
}


void MovementSystem_Update(MovementSystem* movementSystem) {
	MovementComponent* movementComponent = movementSystem->movementComponent;
	RectangleComponent* rectangleComponent = movementSystem->rectangleComponent;
	HatComponent* hatComponent = movementSystem->hatComponent;
	for (uint32 entityIndex = 0; entityIndex < movementComponent->count; entityIndex++) {
		uint32 eid = movementComponent->entityArray[entityIndex];
		if (!Component_HasIndex(movementComponent, eid)) {
			continue;
		}
		if (!Component_HasIndex(rectangleComponent, eid)) {
			continue;
		}
		
		// Get movement values for the entity
		MovementValues* moveValue = &movementComponent->movementValues[eid];
		moveValue->xVelocity 	+= moveValue->xAccel;
		moveValue->yVelocity    += moveValue->yAccel;
		// Check XVelocity Maximum
		if (moveValue->xVelocity >= moveValue->maxXVelocity) {
		  moveValue->xVelocity = moveValue->maxXVelocity;
		} else if (moveValue->xVelocity <= -moveValue->maxXVelocity) {
		  moveValue->xVelocity = -moveValue->maxXVelocity;
		}

		float jump = 1;
		if (Component_HasIndex(hatComponent, eid)) {
		  if (strcmp((&hatComponent->hats[eid].hat)->name, "bunny") == 0)
		  jump = 1.41;
		}
		// Check YVelocity Maximum
		if (moveValue->yVelocity >= moveValue->maxYVelocity) {
		  moveValue->yVelocity = moveValue->maxYVelocity;
		} else if (moveValue->yVelocity <= -14*jump) {
		  moveValue->yVelocity = -14*jump;
		}

		// Get the entity's rectangle
		Rectangle* rectangle = &rectangleComponent->entityRectangles[eid];
		rectangle->x += moveValue->xVelocity;
		rectangle->y += moveValue->yVelocity;
	}
}

void MovementSystem_Free(MovementSystem* movementSystem) {
	movementSystem->movementComponent = nullptr;
	movementSystem->rectangleComponent = nullptr;
	movementSystem->hatComponent = nullptr;
}

