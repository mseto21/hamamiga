#pragma once
#include "Types.h"
/* 
Relies on: MovementComponent.
Usage: If the InputComponent is attached to an entity, the etity will 
accept basic input controls from the user.
*/

// Forward Declarations
struct MovementComponent;
struct InputComponent;
struct HealthComponent;
struct ComponentBag;

struct InputSystem {
	InputComponent* inputComponent;
	MovementComponent* movementComponent; 
	HealthComponent* healthComponent;
	ComponentBag* cBag;
};


void InputSystem_Initialize(InputSystem* inputSystem, ComponentBag* cBag);
void InputSystem_Update(InputSystem* inputSystem, bool keysPressed[], bool keysUp[]);
void InputSystem_Free(InputSystem* inputSystem);