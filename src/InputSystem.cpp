#include "InputSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "InputComponent.h"
#include "RectangleComponent.h"
#include "ComponentBag.h"

#include <SDL.h>
#include <iostream>

void InputSystem_Initialize(InputSystem* inputSystem, ComponentBag* cBag) {
	inputSystem->inputComponent 	= cBag->inputComponent;
	inputSystem->movementComponent 	= cBag->movementComponent;
}

void InputSystem_Update(InputSystem* inputSystem, bool keysPressed[]) {
	InputComponent* inputComponent = inputSystem->inputComponent;
	MovementComponent* movementComponent = inputSystem->movementComponent;;

	for (uint32 entityIndex = 0; entityIndex < inputComponent->count; entityIndex++) {
		uint32 eid = inputComponent->entityArray[entityIndex];
		if (!Component_HasIndex(movementComponent, eid)) {
			continue;
		}

		// Get the movement value
		MovementValues* moveValues = &movementComponent->movementValues[movementComponent->entityArray[entityIndex]];
		if (!moveValues) {
			std::cerr << "Error: No movement values for the input system to use." << std::endl;
			continue;
		}
		
		moveValues->xAccel = 0;
		moveValues->yAccel = 0;
		if (keysPressed[SDLK_w] && moveValues->yVelocity == 0) {
			moveValues->yAccel = -moveValues->accelY;
		}
		if (keysPressed[SDLK_a]) {
		    moveValues->xAccel = -moveValues->accelX;
		}
		/*if (keysPressed[SDLK_s]) {
			moveValues->yAccel = moveValues->accelY;
		}*/
		if (keysPressed[SDLK_d]) {
			moveValues->xAccel = moveValues->accelX;
		}
	}
}
