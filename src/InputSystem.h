#pragma once
/* 
Relies on: MovementComponent.
Usage: If the InputComponent is attached to an entity, the etity will 
accept basic input controls from the user.
*/

// Forward Declarations
struct MovementComponent;
struct InputComponent;
struct HatComponent;
struct ComponentBag;

struct InputSystem {
	InputComponent* inputComponent;
	MovementComponent* movementComponent; 
	HatComponent* hatComponent;
};


void InputSystem_Initialize(InputSystem* inputSystem, ComponentBag* cBag);
void InputSystem_Update(InputSystem* inputSystem, bool keysPressed[], float timestep);