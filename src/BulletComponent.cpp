#include "BulletComponent.h"
#include "PhysicsComponent.h"
#include "AliveComponent.h"
#include "TextureComponent.h"
#include "RectangleComponent.h"
#include "TextureCache.h"

#include <iostream>

void BulletComponent_Add(BulletComponent* bulletComponent, PhysicsComponent* physicsComponent,
	AliveComponent* aliveComponent, TextureComponent* textureComponent,RectangleComponent* rect,
	Rectangle rectPos, uint32 eid, bool team) {

	if (bulletComponent->count < Constants::MaxBullets_){
	//Adding relevant components to a bullet entity
	Component_Add(bulletComponent, eid);
	bulletComponent->bullet[eid].friendly = team;
	PhysicsComponent_Add(physicsComponent, eid, 100);
	AliveComponent_Add(aliveComponent, eid);
	//Texture
	Texture* texture = TextureCache_GetTexture("bullet");
	TextureComponent_Add(textureComponent, eid, texture); //default bullet texture
	bulletComponent->bullet[eid].position.x = rectPos.x + 150;
	bulletComponent->bullet[eid].position.y = rectPos.y + 40;
	bulletComponent->bullet[eid].collided = false;
	//std::cout << "rectpos is: " << rectPos.x << std::endl;
	//std::cout << "rpos y is: " << rectPos.y << std::endl;
	//std::cout << "bullet.position is: " << bullet.position.x << std::endl;
	//std::cout << "bullet.position y is: " << bullet.position.y << std::endl;
	RectangleComponent_Add(rect, eid, bulletComponent->bullet[eid].position.x,
	 bulletComponent->bullet[eid].position.y, texture->w, texture->h);
} else {
	std::cout << "max bullets! count is" << bulletComponent->count << std::endl;
}
}