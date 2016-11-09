#pragma once

//Forward Declarations
struct MovementComponent;
struct FAIComponent;
struct RectangleComponent;
struct ComponentBag;

struct FAISystem {
	MovementComponent* movementComponent;
	RectangleComponent* rectangleComponent;
	FAIComponent* faiComponent;
};

void FAISystem_Initialize(FAISystem* faiSystem, ComponentBag* cBag);
void FAISystem_Update(FAISystem* faiSystem);
void FAISystem_Free(FAISystem* faiSystem);