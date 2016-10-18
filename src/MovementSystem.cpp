#include "MovementSystem.h"
#include "MovementComponent.h"
#include "RectangleComponent.h"
#include "ComponentBag.h"

#include <iostream>

void MovementSystem_Initialize(MovementSystem* movementSystem, ComponentBag* cBag) {
	movementSystem->movementComponent 	=	cBag->movementComponent;
	movementSystem->rectangleComponent  =	cBag->rectangleComponent;
}


void MovementSystem_Update(MovementSystem* movementSystem, float timestep) {
	MovementComponent* movementComponent = movementSystem->movementComponent;
	RectangleComponent* rectangleComponent = movementSystem->rectangleComponent;

	for (uint32 entityIndex = 0; entityIndex < movementComponent->count; entityIndex++) {
		uint32 eid = movementComponent->entityArray[entityIndex];
		if (!Component_HasIndex(rectangleComponent, eid)) {
			continue;
		}
		
		// Get movement values for the entity
		MovementValues* moveValue = &movementComponent->movementValues[eid];
		moveValue->xVelocity 	+= moveValue->xAccel * timestep;
		moveValue->yVelocity    += moveValue->yAccel * timestep;
		
		// Check XVelocity Maximum
		if (moveValue->xVelocity >= moveValue->maxXVelocity && moveValue->xVelocity > 0) {
		  moveValue->xVelocity = moveValue->maxXVelocity;
		} else if (moveValue->xVelocity <= -moveValue->maxXVelocity && moveValue->xVelocity < 0) {
		  moveValue->xVelocity = -moveValue->maxXVelocity;
		}

		// Check YVelocity Maximum
		if (moveValue->yVelocity >= moveValue->maxYVelocity && moveValue->yVelocity > 0) {
		  moveValue->yVelocity = moveValue->maxYVelocity;
		} else if (moveValue->yVelocity <= -moveValue->maxYVelocity && moveValue->yVelocity < 0) {
		  moveValue->yVelocity = -moveValue->maxYVelocity;
		}

		// Get the entity's rectangle
		Rectangle* rectangle = &rectangleComponent->entityRectangles[eid];
		rectangle->x += moveValue->xVelocity;
		rectangle->y += moveValue->yVelocity;

		if (rectangle->x <= 0) {
			rectangle->x = 0;
			moveValue->xVelocity *= -1;
			moveValue->xAccel *= -1;
		}else if (rectangle->x + rectangle->w >= Constants::LevelWidth_) {
			rectangle->x = Constants::LevelWidth_ - rectangle->w;
			moveValue->xVelocity *= -1;
			moveValue->xAccel *= -1;
		}

		if (rectangle->y < 0) {
		  rectangle->y = 0;
		  moveValue->yVelocity = 0;
		}
	}
	
}
