#pragma once

//Forward Declarations
struct MovementComponent;
struct AIComponent;
struct RectangleComponent;
struct AnimationComponent;
struct ComponentBag;

struct AISystem {
	MovementComponent* movementComponent;
	RectangleComponent* rectangleComponent;
	AIComponent* aiComponent;
	AnimationComponent* animationComponent;
};

void AISystem_Initialize(AISystem* aiSystem, ComponentBag* cBag);
void AISystem_Update(AISystem* aiSystem);
void AISystem_Free(AISystem* aiSystem);