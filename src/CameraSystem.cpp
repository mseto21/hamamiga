#include "CameraSystem.h"
#include "CameraComponent.h"
#include "Rectangle.h"
#include "ComponentBag.h"
#include <iostream>

void CameraSystem_Initialize(CameraSystem* cameraSystem, ComponentBag* cBag, Rectangle* playerRect) {
	cameraSystem->cameraComponent =	cBag->cameraComponent;
	cameraSystem->playerRect      = playerRect;
}

void CameraSystem_Update(CameraSystem* cameraSystem) {
	if (!cameraSystem->playerRect) {
		std::cerr << "Error: The camera system is not working." << std::endl;
		return;
	}
	//Center the camera over the player
	SDL_Rect* camera = &cameraSystem->cameraComponent->camera;
	camera->x = ( cameraSystem->playerRect->x + Constants::PlayerWSize_ / 2 ) - Constants::ScreenWidth_ / 2;
	camera->y = ( cameraSystem->playerRect->y + Constants::PlayerHSize_ / 2 ) - Constants::ScreenWidth_ / 2;

	//Keep the camera in bounds
	if( camera->x < 0 )
	{ 
		camera->x = 0;
	}
	if( camera->y < 0 )
	{
		camera->y = 0;
	}
	if( camera->x > Constants::LevelWidth_ - camera->w )
	{
		camera->x = Constants::LevelWidth_  - camera->w;
	}
	if( camera->y > Constants::LevelHeight_  - camera->h )
	{
		camera->y = Constants::LevelHeight_  - camera->h;
	}
}