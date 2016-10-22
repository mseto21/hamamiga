#include "InputSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "InputComponent.h"
#include "RectangleComponent.h"
#include "HatComponent.h"
#include "HealthComponent.h"
#include "ComponentBag.h"

#include <SDL.h>
#include <iostream>

void InputSystem_Initialize(InputSystem* inputSystem, ComponentBag* cBag) {
	inputSystem->inputComponent 	= cBag->inputComponent;
	inputSystem->movementComponent 	= cBag->movementComponent;
	inputSystem->hatComponent       = cBag->hatComponent;
	inputSystem->healthComponent    = cBag->healthComponent;
}

void InputSystem_Update(InputSystem* inputSystem, bool keysPressed[]) {
	InputComponent* inputComponent = inputSystem->inputComponent;
	MovementComponent* movementComponent = inputSystem->movementComponent;;
	HatComponent* hatComponent = inputSystem->hatComponent;
	HealthComponent* healthComponent = inputSystem->healthComponent;
	for (uint32 entityIndex = 0; entityIndex < inputComponent->count; entityIndex++) {
		uint32 eid = inputComponent->entityArray[entityIndex];
		if (!Component_HasIndex(movementComponent, eid)) {
			continue;
		}

		// Get the movement value
		MovementValues* moveValues = &movementComponent->movementValues[eid];
		if (!moveValues) {
			std::cerr << "Error: No movement values for the input system to use." << std::endl;
			continue;
		}
		float jump = 1;
		if (Component_HasIndex(hatComponent, eid)) {
		  jump = (&hatComponent->hats[eid].hat)->getJump();
		}
		moveValues->xAccel = 0;
		moveValues->yAccel = 0;
		if (keysPressed[SDLK_w] && moveValues->grounded) {
			moveValues->yAccel = -moveValues->accelY*jump;
		}
		if (keysPressed[SDLK_a]) {
		    moveValues->xAccel = -moveValues->accelX;
		}
		if (keysPressed[SDLK_s]) {
		    healthComponent->invincible[eid] = !(healthComponent->invincible[eid]);
		}
		if (keysPressed[SDLK_d]) {
			moveValues->xAccel = moveValues->accelX;
		}
	}
}
