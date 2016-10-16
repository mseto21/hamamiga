#include "MovementComponent.h"
#include "RectangleComponent.h"

void MovementSystem_Update(float timestep, MovementComponent* movementComponent, RectangleComponent * rectangleComponent) {
	for (uint32 entityIndex = 0; entityIndex < movementComponent->count; entityIndex++) {
		if (!Component_HasIndex(rectangleComponent, entityIndex)) {
			continue;
		}

		// Get movement values for the entity
		MovementValues* moveValue = &movementComponent->movementValues[movementComponent->entityArray[entityIndex]];
		moveValue->xVelocity      += moveValue->xAccel;
		moveValue->yVelocity      += moveValue->yAccel;
		if (moveValue->xVelocity >= Constants::MaxVX_
		    && moveValue->xVelocity > 0) {
		  moveValue->xVelocity = Constants::MaxVX_;
		} else if (moveValue->xVelocity <= -Constants::MaxVX_
			   && moveValue->xVelocity < 0) {
		  moveValue->xVelocity = -Constants::MaxVX_;
		}
		if (moveValue->yVelocity >= Constants::MaxVY_
		    && moveValue->yVelocity > 0) {
		  moveValue->yVelocity = Constants::MaxVY_;
	        } else if (moveValue->yVelocity <= -Constants::MaxVY_
			   && moveValue->yVelocity < 0) {
		  moveValue->yVelocity = -Constants::MaxVY_;
		}
		

		// Get the entity's rectangle
		Rectangle* rectangle = &rectangleComponent->entityRectangles[rectangleComponent->entityArray[entityIndex]];
		// Move the rectangle appropriately
		rectangle->x += (int)(moveValue->xVelocity * timestep);

		if (rectangle->x <= 0) {
			rectangle->x = 0;
			moveValue->xVelocity *= -1;
			moveValue->xAccel *= -1;
		}

		rectangle->y += (int)(moveValue->yVelocity * timestep);

		if (rectangle->y <= 0) {
			rectangle->y = 0;
		}

		if (rectangle->y + rectangle->h >= Constants::LevelHeight_) {
		        rectangle->y = Constants::LevelHeight_ - rectangle->h;
			moveValue->yVelocity = 0;
		}

		if (rectangle->x + rectangle->w >= Constants::LevelWidth_) {
		        rectangle->x = Constants::LevelWidth_ - rectangle->w;
			moveValue->xVelocity *= -1;
			moveValue->xAccel *= -1;
		}
	}
	
}
