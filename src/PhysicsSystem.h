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
struct GoalComponent;
struct ComponentBag;
struct InteractableComponent;
struct AliveComponent;

struct PhysicsSystem {
	PhysicsComponent* physicsComponent; 
	MovementComponent* movementComponent; 
	RectangleComponent* rectangleComponent; 
	HealthComponent* healthComponent; 
	HatComponent* hatComponent;
	InputComponent* inputComponent;
	GoalComponent* goalComponent;
	InteractableComponent* interactableComponent;
	AliveComponent* aliveComponent;
	TileMap* map;
	ComponentBag* componentBag;
};

void PhysicsSystem_Initialize(PhysicsSystem* physicsSystem, ComponentBag* cBag, TileMap* tileMap);
void PhysicsSystem_Update(PhysicsSystem* physicsSystem);