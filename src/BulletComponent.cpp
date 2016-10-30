#include "BulletComponent.h"

void BulletComponent_Add(BulletComponent* bulletComponent, uint32 eid, Bullet bullet) {
	Component_Add(bulletComponent, eid);
	bulletComponent->bullets[eid] = bullet;
}