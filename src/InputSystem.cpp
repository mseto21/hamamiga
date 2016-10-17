#include "InputSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "InputComponent.h"
#include "RectangleComponent.h"
#include "HatComponent.h"
#include "ComponentBag.h"

#include <SDL.h>
#include <iostream>

void InputSystem_Initialize(InputSystem* inputSystem, ComponentBag* cBag) {
	inputSystem->inputComponent 	= cBag->inputComponent;
	inputSystem->movementComponent 	= cBag->movementComponent;
	inputSystem->rectangleComponent = cBag->rectangleComponent;
	inputSystem->hatComponent 		= cBag->hatComponent;
}

void InputSystem_Update(InputSystem* inputSystem, bool keysPressed[]) {
	InputComponent* inputComponent = inputSystem->inputComponent;
	MovementComponent* movementComponent = inputSystem->movementComponent;
	RectangleComponent* rectangleComponent = inputSystem->rectangleComponent;
	HatComponent* hatComponent = inputSystem->hatComponent;

	for (uint32 entityIndex = 0; entityIndex < inputComponent->count; entityIndex++) {
		if (!Component_HasIndex(movementComponent, inputComponent->entityArray[entityIndex])) {
			continue;
		}
		int jump = 1;
		if (Component_HasIndex(hatComponent, inputComponent->entityArray[entityIndex])) {
		  Hat* hat = &hatComponent->hats[hatComponent->entityArray[entityIndex]].hat;
		  jump = hat->getJump();
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
			moveValues->yVelocity = -Constants::Jump_*jump;
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
