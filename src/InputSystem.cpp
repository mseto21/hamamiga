#include "InputSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "InputComponent.h"
#include "RectangleComponent.h"
#include "HatComponent.h"
#include "HealthComponent.h"
#include "BulletComponent.h"
#include "PhysicsComponent.h"
#include "AliveComponent.h"
#include "TextureComponent.h"
//#include "RectangleComponent.h"
#include "EntityCache.h"
#include "ComponentBag.h"

#include <SDL.h>
#include <iostream>

void InputSystem_Initialize(InputSystem* inputSystem, ComponentBag* cBag) {
	inputSystem->inputComponent 	= cBag->inputComponent;
	inputSystem->movementComponent 	= cBag->movementComponent;
	inputSystem->hatComponent       = cBag->hatComponent;
	inputSystem->healthComponent    = cBag->healthComponent;
	inputSystem->bulletComponent  	= cBag->bulletComponent;
	inputSystem->aliveComponent  		= cBag->aliveComponent;
	inputSystem->physicsComponent  	= cBag->physicsComponent;
	inputSystem->textureComponent  	= cBag->textureComponent;
	inputSystem->rectangleComponent = cBag->rectangleComponent;
}

void InputSystem_Update(InputSystem* inputSystem, bool keysPressed[], bool keysUp[], uint16 numPressed[]) {
	InputComponent* inputComponent = inputSystem->inputComponent;
	MovementComponent* movementComponent = inputSystem->movementComponent;;
	HatComponent* hatComponent = inputSystem->hatComponent;
	HealthComponent* healthComponent = inputSystem->healthComponent;
	BulletComponent* bulletComponent = inputSystem->bulletComponent;
	PhysicsComponent* physicsComponent = inputSystem->physicsComponent;
	AliveComponent* aliveComponent = inputSystem->aliveComponent;
	TextureComponent* textureComponent = inputSystem->textureComponent;
	RectangleComponent* rectangleComponent = inputSystem->rectangleComponent;

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

		if (eid == Constants::PlayerIndex_ && Component_HasIndex(hatComponent, eid) &&
			(strcmp(hatComponent->hats[eid].hat.effect, "powpow") == 0) && 
			bulletComponent->count < Constants::MaxBullets_){
			if (keysPressed[SDLK_SPACE] && keysUp[SDLK_SPACE]) {
				//caster's position
				Rectangle rect = rectangleComponent->entityRectangles[eid];
				Entity* newBullet = EntityCache_GetNewEntity();
				BulletComponent_Add(inputSystem->bulletComponent, inputSystem->physicsComponent,
				inputSystem->aliveComponent, inputSystem->textureComponent, inputSystem->rectangleComponent,
				rect, newBullet->eid);
				std::cout << "created bullet entity! with eid " << newBullet->eid <<std::endl;
				//numPressed[SDLK_SPACE] = 0;
				keysUp[SDLK_SPACE % Constants::NumKeys_] = false;

			}
		}
		//}

		if (keysPressed[SDLK_p]) {
			// Drop regular hat.
		} else if (keysPressed[SDLK_o]) {
			// Drop glamour hat.
		}
	}
}

void InputSystem_Free(InputSystem* inputSystem) {
	inputSystem->inputComponent = nullptr;
	inputSystem->movementComponent = nullptr;
	inputSystem->hatComponent = nullptr;
	inputSystem->healthComponent = nullptr;
	inputSystem->bulletComponent = nullptr;
	inputSystem->physicsComponent = nullptr;
	inputSystem->aliveComponent = nullptr;
	inputSystem->textureComponent = nullptr;
}
