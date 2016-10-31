#include "Bullet.h"
#include "Component.h"

struct PhysicsComponent;
struct AliveComponent;
struct TextureComponent;

struct BulletComponent : public Component {
	Bullet bullets[Constants::MaxBullets_];
	bool activated;
};

void BulletComponent_Add(BulletComponent* bulletComponent, PhysicsComponent*,
	AliveComponent*, TextureComponent*, uint32 eid, Bullet bullet);
