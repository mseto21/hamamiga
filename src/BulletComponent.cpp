#include "BulletComponent.h"
#include "PhysicsComponent.h"
#include "AliveComponent.h"
#include "TextureComponent.h"
#include "MovementComponent.h"
#include "RectangleComponent.h"
#include "TextureCache.h"
#include "Interactions.h"

#include <SDL.h>


void BulletComponent_Add(BulletComponent* bulletComponent, PhysicsComponent* physicsComponent,
	AliveComponent* aliveComponent, TextureComponent* textureComponent, MovementComponent* movementComponent,
			 RectangleComponent* rect, Rectangle rectPos, uint32 eid, bool team, bool left, int txtr) {

	//Adding relevant components to a bullet entity
	Component_Add(bulletComponent, eid);
	bulletComponent->bullet[eid].friendly = team;
	bulletComponent->bullet[eid].life = 0;
	PhysicsComponent_Add(physicsComponent, eid, 100);
	AliveComponent_Add(aliveComponent, eid);
	//Texture
	Texture* texture;
	if (txtr == 0) {
	  texture = TextureCache_GetTexture("bullet");
	  bulletComponent->type[eid] = HatTypes_Cowboy;
	} else {
	  texture = TextureCache_GetTexture("knife");
	  bulletComponent->type[eid] = HatTypes_Chef;
	}
	if (left == true) {
		texture->flip = SDL_FLIP_HORIZONTAL;
	} else {
		texture->flip = SDL_FLIP_NONE;
	}
	TextureComponent_Add(textureComponent, eid, texture); //default bullet texture
	float startX = rectPos.x;
	float startY = rectPos.y;
	bulletComponent->bullet[eid].collided = false;
	bulletComponent->bullet[eid].left = left;//false if going right
	if (!left)
		RectangleComponent_Add(rect, eid, startX + 50, startY + 40, texture->w, texture->h);
	else
		RectangleComponent_Add(rect, eid, startX - texture->w, startY + 40, texture->w, texture->h);
	//Movement
	if (txtr == 0) {
	  MovementComponent_Add(movementComponent, eid, 7, 0, 1.5, 1.0);//default bullet speed
	} else {
	  MovementComponent_Add(movementComponent, eid, 9, 14, 1.5, 1.0);
	  movementComponent->movementValues[eid].yVelocity = -7
	    + movementComponent->movementValues[Constants::PlayerIndex_].yVelocity;
	  movementComponent->movementValues[eid].xVelocity +=
	    movementComponent->movementValues[Constants::PlayerIndex_].xVelocity;
	}
}
