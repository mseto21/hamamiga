#include "CameraComponent.h"

void CameraComponent_Add(CameraComponent* cameraComponent, uint32 eid) {
	cameraComponent->camera.x = 0;
	cameraComponent->camera.y = 0;
	cameraComponent->camera.w = Constants::ScreenWidth_;
	cameraComponent->camera.h = Constants::ScreenHeight_;
	Component_Add(cameraComponent, eid);
}