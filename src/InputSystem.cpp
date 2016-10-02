#include "InputSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "InputComponent.h"

#include <SDL.h>

void InputSystem_Update(bool keysPressed[], InputComponent* inputComponent, MovementComponent* movementComponent) {
	for (uint32 entityIndex = 0; entityIndex < inputComponent->count; entityIndex++) {
		if (!Component_HasIndex(movementComponent, entityIndex)) {
			continue;
		}

		// Get the movement value
		MovementValues* moveValues = &movementComponent->movementValues[movementComponent->entityArray[entityIndex]];
		moveValues->xVelocity = 0;
		moveValues->yVelocity = 0;
		if (keysPressed[SDLK_w]) {
			moveValues->xVelocity = -Constants::StepSize_;
		}
		if (keysPressed[SDLK_a]) {
			moveValues->yVelocity = -Constants::StepSize_;
		}
		if (keysPressed[SDLK_s]) {
			moveValues->yVelocity = Constants::StepSize_;
		}
		if (keysPressed[SDLK_d]) {
			moveValues->xVelocity = Constants::StepSize_;
		}
	}
}