#include "BulletComponent.h"
#include "PhysicsComponent.h"
#include "AliveComponent.h"
#include "TextureComponent.h"
#include "RectangleComponent.h"
#include "TextureCache.h"

#include <iostream>

void BulletComponent_Add(BulletComponent* bulletComponent, PhysicsComponent* physicsComponent,
	AliveComponent* aliveComponent, TextureComponent* textureComponent,RectangleComponent* rect,
	Rectangle rectPos, uint32 eid) {
	//Adding relevant components to a bullet entity
	Component_Add(bulletComponent, eid);
	PhysicsComponent_Add(physicsComponent, eid, 100);
	AliveComponent_Add(aliveComponent, eid);
	//Texture
	Texture* texture = TextureCache_GetTexture("bullet");
	TextureComponent_Add(textureComponent, eid, texture); //default bullet texture
	bulletComponent->bullet.position.x = rectPos.x + 150;
	bulletComponent->bullet.position.y = rectPos.y + 40;
	//std::cout << "rectpos is: " << rectPos.x << std::endl;
	//std::cout << "rpos y is: " << rectPos.y << std::endl;
	//std::cout << "bullet.position is: " << bullet.position.x << std::endl;
	//std::cout << "bullet.position y is: " << bullet.position.y << std::endl;
	RectangleComponent_Add(rect, eid, bulletComponent->bullet.position.x, bulletComponent->bullet.position.y, texture->w, texture->h);
}