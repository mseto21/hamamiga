#pragma once
#include "Constants.h"
// Forward Declarations
struct Rectangle;
struct ComponentBag;
struct CameraComponent;
struct RectangleComponent;

struct CameraSystem {
	CameraComponent* cameraComponent;
	RectangleComponent* rectangleComponent;
};

void CameraSystem_Initialize(CameraSystem* cameraSystem, ComponentBag* cBag);
void CameraSystem_Update(CameraSystem* cameraSystem);