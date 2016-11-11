#pragma once
#include "Types.h"

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