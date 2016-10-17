#include "CameraSystem.h"
#include "CameraComponent.h"
#include "RectangleComponent.h"
#include "ComponentBag.h"
#include <iostream>

void CameraSystem_Initialize(CameraSystem* cameraSystem, ComponentBag* cBag) {
	cameraSystem->cameraComponent =	cBag->cameraComponent;
	cameraSystem->rectangleComponent      = cBag->rectangleComponent;
}

void CameraSystem_Update(CameraSystem* cameraSystem) {
	CameraComponent* cameraComponent = cameraSystem->cameraComponent;
	RectangleComponent* rectangleComponent = cameraSystem->rectangleComponent;

	for (uint32 entityIndex = 0; entityIndex < cameraComponent->count; entityIndex++) {
		uint32 eid = cameraComponent->entityArray[entityIndex];
		if (!Component_HasIndex(rectangleComponent, eid)) {
			continue;
		}

		Rectangle rectangle = rectangleComponent->entityRectangles[eid];
		SDL_Rect* camera = &cameraSystem->cameraComponent->camera;
		camera->x = ( rectangle.x + Constants::PlayerWSize_ / 2 ) - Constants::ScreenWidth_ / 2;
		camera->y = ( rectangle.y + Constants::PlayerHSize_ / 2 ) - Constants::ScreenWidth_ / 2;

		//Keep the camera in bounds
		if (camera->x < 0) { 
			camera->x = 0;
		}

		if (camera->y < 0) {
			camera->y = 0;
		}

		if (camera->x > Constants::LevelWidth_ - camera->w) {
			camera->x = Constants::LevelWidth_  - camera->w;
		}
		
		if (camera->y > Constants::LevelHeight_  - camera->h) {
			camera->y = Constants::LevelHeight_  - camera->h;
		}
	}
}