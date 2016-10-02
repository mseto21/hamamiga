#include "MovementComponent.h"
#include "RectangleComponent.h"

void MovementSystem_Update(float timestep, MovementComponent* movementComponent, RectangleComponent * rectangleComponent) {
	for (uint32 entityIndex = 0; entityIndex < movementComponent->count; entityIndex++) {
		if (!Component_HasIndex(rectangleComponent, entityIndex)) {
			continue;
		}

		// Get movement values for the entity
		MovementValues* moveValue = &movementComponent->movementValues[movementComponent->entityArray[entityIndex]];
		float xVelocity = moveValue->xVelocity;
		float yVelocity = moveValue->yVelocity;
		float xAccel 	= moveValue->xAccel;
		float yAccel	= moveValue->yAccel;
		(void) xAccel;
		(void) yAccel;

		// Get the entity's rectangle
		Rectangle* rectangle = &rectangleComponent->entityRectangles[rectangleComponent->entityArray[entityIndex]];

		// Move the rectangle appropriately
		rectangle->x += (int)(xVelocity * timestep);

		if (rectangle->x <= 0) {
			rectangle->x = 0;
		}

		rectangle->y += (int)(yVelocity * timestep);

		if (rectangle->y <= 0) {
			rectangle->y = 0;
		}
	}
}