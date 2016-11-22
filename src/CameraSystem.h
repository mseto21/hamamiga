#pragma once
#include "Constants.h"
// Forward Declarations
struct ComponentBag;
struct CameraComponent;
struct RectangleComponent;
struct Zone;

struct CameraSystem {
	CameraComponent* cameraComponent;
	RectangleComponent* rectangleComponent;
	Zone* zone;
};

void CameraSystem_Initialize(CameraSystem* cameraSystem, ComponentBag* cBag, Zone* zone);
void CameraSystem_Update(CameraSystem* cameraSystem);
void CameraSystem_Free(CameraSystem* cameraSystem);