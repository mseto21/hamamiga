#include "BulletComponent.h"
#include "PhysicsComponent.h"
#include "AliveComponent.h"
#include "TextureComponent.h"
#include "RectangleComponent.h"
#include "TextureCache.h"

void BulletComponent_Add(BulletComponent* bulletComponent, PhysicsComponent* physicsComponent,
	AliveComponent* aliveComponent, TextureComponent* textureComponent,RectangleComponent* rect,
	Rectangle rectPos, uint32 eid, Bullet bullet) {
	//Adding relevant components to a bullet entity
	Component_Add(bulletComponent, eid);
	PhysicsComponent_Add(physicsComponent, eid, 100);
	AliveComponent_Add(aliveComponent, eid);
	//Texture
	Texture* texture = TextureCache_GetTexture("bullet");
	TextureComponent_Add(textureComponent, eid, texture); //default bullet texture
	RectangleComponent_Add(rect, eid, rectPos.x + 150, rectPos.y + 40, texture->w, texture->h);
}