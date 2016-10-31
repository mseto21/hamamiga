#include "InputSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "InputComponent.h"
#include "RectangleComponent.h"
#include "HatComponent.h"
#include "HealthComponent.h"
#include "BulletComponent.h"
#include "ComponentBag.h"

#include <SDL.h>
#include <iostream>

void InputSystem_Initialize(InputSystem* inputSystem, ComponentBag* cBag) {
	inputSystem->inputComponent 	= cBag->inputComponent;
	inputSystem->movementComponent 	= cBag->movementComponent;
	inputSystem->hatComponent       = cBag->hatComponent;
	inputSystem->healthComponent    = cBag->healthComponent;
	inputSystem->bulletComponent  	= cBag->bulletComponent;
}

void InputSystem_Update(InputSystem* inputSystem, bool keysPressed[], bool keysUp[]) {
	InputComponent* inputComponent = inputSystem->inputComponent;
	MovementComponent* movementComponent = inputSystem->movementComponent;;
	HatComponent* hatComponent = inputSystem->hatComponent;
	HealthComponent* healthComponent = inputSystem->healthComponent;
	BulletComponent* bulletComponent = inputSystem->bulletComponent;
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
		moveValues->xAccel = 0;
		moveValues->yAccel = 0;
		if (keysPressed[SDLK_w] && moveValues->grounded) {
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
		else if (keysPressed[SDLK_e]) {
			inputComponent->interact[eid] = true;
		} else if (keysUp[SDLK_e]) {
			inputComponent->interact[eid] = false;
		}
		//Checking bullet activation
		if (Component_HasIndex(bulletComponent, eid) && eid == Constants::PlayerIndex_){
			if (keysPressed[SDLK_SPACE]) {
				bulletComponent->activated = true;//bullets[eid].activated = true;
				std::cout << "activated bullets with eid: " << eid  << " count is "<< inputComponent->count<< std::endl;
			}
		}
	}



}