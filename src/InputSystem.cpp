#include "InputSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "InputComponent.h"
#include "HealthComponent.h"
#include "ComponentBag.h"
#include "InteractionTypes.h"

#include <SDL.h>
#include <iostream>

void InputSystem_Initialize(InputSystem* inputSystem, ComponentBag* cBag) {
	inputSystem->inputComponent 	= cBag->inputComponent;
	inputSystem->movementComponent 	= cBag->movementComponent;
	inputSystem->healthComponent    = cBag->healthComponent;
	inputSystem->cBag 				= cBag;
}

void InputSystem_Update(InputSystem* inputSystem, bool keysPressed[], bool keysUp[]) {
	InputComponent* inputComponent = inputSystem->inputComponent;
	MovementComponent* movementComponent = inputSystem->movementComponent;;
	HealthComponent* healthComponent = inputSystem->healthComponent;

	for (uint32 entityIndex = 0; entityIndex < inputComponent->count; entityIndex++) {
		uint32 eid = inputComponent->entityArray[entityIndex];
		if (!Component_HasIndex(inputComponent, eid)) {
			continue;
		}
		if (!Component_HasIndex(movementComponent, eid)) {
			continue;
		}

		// Get the movement value
		MovementValues* moveValues = &movementComponent->movementValues[eid];
		if (!moveValues) {
			std::cerr << "Error: No movement values for the input system to use." << std::endl;
			continue;
		}
		moveValues->xAccel = 0;
		moveValues->yAccel = 0;
		if (keysPressed[SDLK_w] && (moveValues->grounded || moveValues->flying)) {
			moveValues->yAccel = -moveValues->accelY;
		}
		if (keysPressed[SDLK_a]) {
		    moveValues->xAccel = -moveValues->accelX;
		}
		if (keysPressed[SDLK_i]) {
		    healthComponent->invincible[eid] = !(healthComponent->invincible[eid]);
		}
		if (keysPressed[SDLK_d]) {
			moveValues->xAccel = moveValues->accelX;
		}
		if (keysPressed[SDLK_s] && moveValues->flying) {
		    moveValues->yAccel = moveValues->accelY;
		}
		
		if (keysPressed[SDLK_e]) {
			inputComponent->interact[eid] = true;
		} else if (keysUp[SDLK_e]) {
			inputComponent->interact[eid] = false;
		}

		if (keysPressed[SDLK_SPACE] && keysUp[SDLK_SPACE]) {
			Interaction_PlayEventInteraction(eid, inputSystem->cBag);
			keysUp[SDLK_SPACE % Constants::NumKeys_] = false;
		}

		if (keysPressed[SDLK_q] && keysUp[SDLK_q]) {
			Interaction_RemoveHatInteraction(eid, inputSystem->cBag);
			keysUp[SDLK_q] = false;
		}
	}
}

void InputSystem_Free(InputSystem* inputSystem) {
	inputSystem->inputComponent = nullptr;
	inputSystem->movementComponent = nullptr;
	inputSystem->healthComponent = nullptr;
	inputSystem->cBag = nullptr;
}
