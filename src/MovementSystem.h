#pragma once

// Forward Declarations
struct MovementComponent;
struct RectangleComponent;
struct HatComponent;
struct ComponentBag;

struct MovementSystem {
	MovementComponent* movementComponent;
	RectangleComponent * rectangleComponent;
	HatComponent* hatComponent;
};

void MovementSystem_Initialize(MovementSystem* movementSystem, ComponentBag* cBag);
void MovementSystem_Update(MovementSystem* movementSystem);