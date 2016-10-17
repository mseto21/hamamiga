#pragma once

// Forward Declarations
struct MovementComponent;
struct RectangleComponent;
struct ComponentBag;

struct MovementSystem {
	MovementComponent* movementComponent;
	RectangleComponent * rectangleComponent;
};

void MovementSystem_Initialize(MovementSystem* movementSystem, ComponentBag* cBag);
void MovementSystem_Update(MovementSystem* movementSystem, float timestep);