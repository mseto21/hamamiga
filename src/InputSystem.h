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
struct HatComponent;
struct HealthComponent;
struct BulletComponent;
struct PhysicsComponent;
struct AliveComponent;
struct TextureComponent;
struct RectangleComponent;
struct ComponentBag;

struct InputSystem {
	InputComponent* inputComponent;
	MovementComponent* movementComponent; 
	HatComponent* hatComponent;
	HealthComponent* healthComponent;
	BulletComponent* bulletComponent;
	PhysicsComponent* physicsComponent;
	AliveComponent* aliveComponent;
	TextureComponent* textureComponent;
	RectangleComponent* rectangleComponent;
};


void InputSystem_Initialize(InputSystem* inputSystem, ComponentBag* cBag);
void InputSystem_Update(InputSystem* inputSystem, bool keysPressed[], bool keysUp[]);
void InputSystem_Free(InputSystem* inputSystem);