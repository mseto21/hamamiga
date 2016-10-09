#pragma once
/* 
Relies on: MovementComponent.
Usage: If the PhysicsComponent is attached to an entity, the etity will 
be affected by gravity controls from the user.
*/

// Forward Declarations
struct PhysicsComponent;
struct MovementComponent;

void PhysicsSystem_Update(PhysicsComponent* physicsComponent, MovementComponent* movementComponent);