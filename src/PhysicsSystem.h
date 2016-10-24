#pragma once
/* 
Relies on: MovementComponent.
Usage: If the PhysicsComponent is attached to an entity, the etity will 
be affected by gravity controls from the user.
*/

// Forward Declarations
struct PhysicsComponent;
struct MovementComponent;
struct RectangleComponent;
struct HealthComponent;
struct HatComponent;
struct InputComponent;
struct TileMap;
struct ComponentBag;
struct AliveComponent;

struct PhysicsSystem {
	PhysicsComponent* physicsComponent; 
	MovementComponent* movementComponent; 
	RectangleComponent* rectangleComponent; 
	HealthComponent* healthComponent; 
	HatComponent* hatComponent;
	TileMap* map;
	InputComponent* inputComponent;
	AliveComponent* aliveComponent;
};

void PhysicsSystem_Initialize(PhysicsSystem* physicsSystem, ComponentBag* cBag, TileMap* tileMap);
int PhysicsSystem_Update(PhysicsSystem* physicsSystem);