#include "BulletComponent.h"
#include "PhysicsComponent.h"
#include "AliveComponent.h"
#include "TextureComponent.h"
#include "MovementComponent.h"
#include "RectangleComponent.h"
#include "TextureCache.h"

void BulletComponent_Add(BulletComponent* bulletComponent, PhysicsComponent* physicsComponent,
	AliveComponent* aliveComponent, TextureComponent* textureComponent, MovementComponent* movementComponent,
	RectangleComponent* rect, Rectangle rectPos, uint32 eid, bool team) {
	if (bulletComponent->count < Constants::MaxBullets_){
	//Adding relevant components to a bullet entity
	Component_Add(bulletComponent, eid);
	bulletComponent->bullet[eid].friendly = team;
	PhysicsComponent_Add(physicsComponent, eid, 100);
	AliveComponent_Add(aliveComponent, eid);
	//Texture
	Texture* texture = TextureCache_GetTexture("bullet");
	TextureComponent_Add(textureComponent, eid, texture); //default bullet texture
	float startX = rectPos.x;
	float startY = rectPos.y;
	bulletComponent->bullet[eid].collided = false;
	RectangleComponent_Add(rect, eid, startX + 50, startY + 40, texture->w, texture->h);
	//Movement
	MovementComponent_Add(movementComponent, eid, 4, 0, 1.0, 1.0);//default bullet speed
	}
}