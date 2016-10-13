#include "CameraComponent.h"
#include "Rectangle.h"

void CameraSystem_Update(CameraComponent * cameraComponent, Rectangle * rect)
{
//Center the camera over the player
	SDL_Rect* camera = &cameraComponent->camera;
	camera->x = ( rect->x + Constants::PlayerWSize_ / 2 ) - Constants::ScreenWidth_ / 2;
	camera->y = ( rect->y + Constants::PlayerHSize_ / 2 ) - Constants::ScreenWidth_ / 2;

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