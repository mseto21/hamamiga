#pragma once

// Forward Declarations
struct PhysicsComponent;
struct MovementComponent;
struct RectangleComponent;
struct HealthComponent;
struct HatComponent;
struct InputComponent;
struct TileMap;
struct ComponentBag;
struct ComponentBag;
struct InteractableComponent;
struct AliveComponent;
struct BulletComponent;

struct PhysicsSystem {
	PhysicsComponent* physicsComponent; 
	MovementComponent* movementComponent; 
	RectangleComponent* rectangleComponent; 
	HealthComponent* healthComponent; 
	HatComponent* hatComponent;
	BulletComponent* bulletComponent;
	InputComponent* inputComponent;
	InteractableComponent* interactableComponent;
	AliveComponent* aliveComponent;
	TileMap* map;
	ComponentBag* componentBag;
};

void PhysicsSystem_Initialize(PhysicsSystem* physicsSystem, ComponentBag* cBag, TileMap* tileMap);
void PhysicsSystem_Update(PhysicsSystem* physicsSystem);
void PhysicsSystem_Free(PhysicsSystem* physicsSystem);