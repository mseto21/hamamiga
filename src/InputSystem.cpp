#include "InputSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "InputComponent.h"
#include "HealthComponent.h"
#include "ComponentBag.h"
#include "Interactions.h"

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
	MovementComponent* movementComponent = inputSystem->movementComponent;
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
		if ((keysPressed[SDLK_UP % Constants::NumKeys_]  || keysPressed[SDLK_SPACE])
		    && (moveValues->grounded || moveValues->flying)) {
			moveValues->yAccel = -moveValues->accelY;
		}
		if (keysPressed[SDLK_LEFT % Constants::NumKeys_]) {
		    moveValues->xAccel = -moveValues->accelX;
		}
		if (keysPressed[SDLK_i]) {
		    healthComponent->invincible[eid] = 1000000;
		}

		if (keysPressed[SDLK_k]){
			healthComponent->health[eid] = 0;
		}
		if (keysPressed[SDLK_RIGHT % Constants::NumKeys_]) {

			moveValues->xAccel = moveValues->accelX;
		}
		if (keysPressed[SDLK_DOWN % Constants::NumKeys_] && (moveValues->flying || (moveValues->drunk && moveValues->grounded))) {
		  
		    moveValues->yAccel = moveValues->accelY;
		}
		
		if (keysPressed[SDLK_x]) {
			inputComponent->interact[eid] = true;
		} else if (keysUp[SDLK_x]) {
			inputComponent->interact[eid] = false;
		}

		if (keysPressed[SDLK_z] && keysUp[SDLK_z]) {
			Interaction_PlayEventInteraction(eid, inputSystem->cBag);
			keysUp[SDLK_z % Constants::NumKeys_] = false;
		}

		if (keysPressed[SDLK_c] && keysUp[SDLK_c]) {
			Interaction_RemoveHatInteraction(eid, inputSystem->cBag);
			keysUp[SDLK_c] = false;
		}
	}
}

void InputSystem_Free(InputSystem* inputSystem) {
	inputSystem->inputComponent = nullptr;
	inputSystem->movementComponent = nullptr;
	inputSystem->healthComponent = nullptr;
	inputSystem->cBag = nullptr;
}
