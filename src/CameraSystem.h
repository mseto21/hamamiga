#pragma once
#include "Constants.h"
// Forward Declarations
struct CameraComponent;
struct Rectangle;
struct ComponentBag;

struct CameraSystem {
	CameraComponent * cameraComponent;
	Rectangle * playerRect;
};

void CameraSystem_Initialize(CameraSystem* cameraSystem, ComponentBag* cBag, Rectangle* playerRect);
void CameraSystem_Update(CameraSystem* cameraSystem);