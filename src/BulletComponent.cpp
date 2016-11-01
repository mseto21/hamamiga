#include "BulletComponent.h"
#include "PhysicsComponent.h"
#include "AliveComponent.h"
#include "TextureComponent.h"
#include "TextureCache.h"

void BulletComponent_Add(BulletComponent* bulletComponent, PhysicsComponent* physicsComponent,
	AliveComponent* aliveComponent, TextureComponent* textureComponent, uint32 eid, Bullet bullet) {
	//Adding relevant components to a bullet entity
//	Component_Add(bulletComponent, eid);
	//PhysicsComponent_Add(physicsComponent, eid, 100);
	//AliveComponent_Add(aliveComponent, eid);
	//TextureComponent_Add(textureComponent, eid, TextureCache_GetTexture("bullet")); //default bullet texture
	//bulletComponent->bullets[eid] = bullet;
	//bulletComponent->activated = true;
}