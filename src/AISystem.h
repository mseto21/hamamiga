#pragma once

//Forward Declarations
struct MovementComponent;
struct AIComponent;
struct RectangleComponent;
struct ComponentBag;

struct AISystem {
	MovementComponent* movementComponent;
	RectangleComponent* rectangleComponent;
	AIComponent* aiComponent;
};

void AISystem_Initialize(AISystem* aiSystem, ComponentBag* cBag);
void AISystem_Update(AISystem* aiSystem, float timeStep);
