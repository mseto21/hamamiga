#include "Bullet.h"
#include "Component.h"

struct BulletComponent : public Component {
	Bullet bullets[Constants::MaxBullets_];
	bool activated;
};

void BulletComponent_Add(BulletComponent* bulletComponent, uint32 eid, Bullet bullet);
