#include "InputSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "InputComponent.h"
#include "RectangleComponent.h"

#include <SDL.h>
#include <iostream>

void InputSystem_Update(bool keysPressed[], InputComponent* inputComponent, MovementComponent* movementComponent, RectangleComponent* rectangleComponent) {
	for (uint32 entityIndex = 0; entityIndex < inputComponent->count; entityIndex++) {
		if (!Component_HasIndex(movementComponent, entityIndex)) {
			continue;
		}
		Rectangle* rectangle = &rectangleComponent->entityRectangles[rectangleComponent->entityArray[entityIndex]];
		// Get the movement value
		MovementValues* moveValues = &movementComponent->movementValues[movementComponent->entityArray[entityIndex]];
		moveValues->xAccel = 0;
		moveValues->yAccel = 0;
		if (keysPressed[SDLK_w] && (rectangle->y + rectangle->h) >= Constants::ScreenHeight_) {
			moveValues->yVelocity = -Constants::Jump_;
		}
		if (keysPressed[SDLK_a]) {
		        moveValues->xAccel = -Constants::Accel_;
		}
		if (keysPressed[SDLK_s]) {
			moveValues->yAccel = Constants::Accel_;
		}
		if (keysPressed[SDLK_d]) {
			moveValues->xAccel = Constants::Accel_;
		}
	}
}
