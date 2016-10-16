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
struct TileMap;

void PhysicsSystem_Update(float timestep, PhysicsComponent* physicsComponent, MovementComponent* movementComponent, RectangleComponent* rectangleComponent, HealthComponent* healthComponent, HatComponent* hatComponent, TileMap* map);