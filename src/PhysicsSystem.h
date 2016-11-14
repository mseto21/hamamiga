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
struct FAIComponent;
struct AIComponent;
struct BulletComponent;
struct Game;

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
	FAIComponent* faiComponent;
	AIComponent* aiComponent;
	TileMap* map;
	ComponentBag* componentBag;
	Game* game;
};

void PhysicsSystem_Initialize(PhysicsSystem* physicsSystem, ComponentBag* cBag, TileMap* tileMap, Game* game);
void PhysicsSystem_Update(PhysicsSystem* physicsSystem);
void PhysicsSystem_Free(PhysicsSystem* physicsSystem);