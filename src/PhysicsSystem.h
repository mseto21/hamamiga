#pragma once

// Forward Declarations
struct PhysicsComponent;
struct MovementComponent;
struct RectangleComponent;
struct HealthComponent;
struct InputComponent;
struct TileMap;
struct ComponentBag;
struct ComponentBag;
struct InteractableComponent;
struct AIComponent;
struct DamageComponent;
struct BulletComponent;
struct Zone;

struct PhysicsSystem {
	PhysicsComponent* physicsComponent; 
	MovementComponent* movementComponent; 
	RectangleComponent* rectangleComponent; 
	HealthComponent* healthComponent; 
	BulletComponent* bulletComponent;
	InputComponent* inputComponent;
	InteractableComponent* interactableComponent;
	AIComponent* aiComponent;
	DamageComponent* damageComponent;
	TileMap* map;
	ComponentBag* componentBag;
	Zone* zone;
};

void PhysicsSystem_Initialize(PhysicsSystem* physicsSystem, ComponentBag* cBag, TileMap* tileMap,Zone* zone);
void PhysicsSystem_Update(PhysicsSystem* physicsSystem);
void PhysicsSystem_Free(PhysicsSystem* physicsSystem);