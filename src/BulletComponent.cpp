#include "BulletComponent.h"
#include "PhysicsComponent.h"
#include "AliveComponent.h"

void BulletComponent_Add(BulletComponent* bulletComponent, PhysicsComponent* physicsComponent,
	AliveComponent* aliveComponent, uint32 eid, Bullet bullet) {
	//Adding relevant components to a bullet entity
	Component_Add(bulletComponent, eid);
	PhysicsComponent_Add(physicsComponent, eid, 100);
	AliveComponent_Add(aliveComponent, eid);
	bulletComponent->bullets[eid] = bullet;
	bulletComponent->activated = true;
}