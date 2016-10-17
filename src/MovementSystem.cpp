#include "MovementComponent.h"
#include "RectangleComponent.h"

void MovementSystem_Update(float timestep, MovementComponent* movementComponent, RectangleComponent * rectangleComponent) {
	for (uint32 entityIndex = 0; entityIndex < movementComponent->count; entityIndex++) {
		if (!Component_HasIndex(rectangleComponent, movementComponent->entityArray[entityIndex])) {
			continue;
		}
		

		// Get movement values for the entity
		MovementValues* moveValue = &movementComponent->movementValues[movementComponent->entityArray[entityIndex]];
		moveValue->xVelocity      += moveValue->xAccel * timestep;
		moveValue->yVelocity      += moveValue->yAccel * timestep;
		int slow = 0;
		if (entityIndex == Constants::PlayerIndex_) {
		  slow = 4;
		}

		if (moveValue->xVelocity >= (Constants::MaxVX_ - slow) && moveValue->xVelocity > 0) {
		  moveValue->xVelocity = Constants::MaxVX_ - slow;
		} else if (moveValue->xVelocity <= (-Constants::MaxVX_ + slow) && moveValue->xVelocity < 0) {
		  moveValue->xVelocity = -Constants::MaxVX_ + slow;
		}

		if (moveValue->yVelocity >= Constants::MaxVY_ && moveValue->yVelocity > 0) {
		  moveValue->yVelocity = Constants::MaxVY_;
	    } else if (moveValue->yVelocity <= -Constants::MaxVY_ && moveValue->yVelocity < 0) {
		  moveValue->yVelocity = -Constants::MaxVY_;
		}
		

		// Get the entity's rectangle
		Rectangle* rectangle = &rectangleComponent->entityRectangles[rectangleComponent->entityArray[entityIndex]];
		// Move the rectangle appropriately
		rectangle->x += (int)(moveValue->xVelocity);

		if (rectangle->x <= 0) {
			rectangle->x = 0;
			moveValue->xVelocity *= -1;
			moveValue->xAccel *= -1;
		}

		rectangle->y += (int)(moveValue->yVelocity);

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
