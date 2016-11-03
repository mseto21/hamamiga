#include "Bullet.h"
#include "Component.h"

struct PhysicsComponent;
struct AliveComponent;
struct TextureComponent;
struct RectangleComponent;
struct Rectangle;

struct BulletComponent : public Component {
	Bullet bullets[Constants::MaxBullets_];
	bool activated;
};

void BulletComponent_Add(BulletComponent* bulletComponent, PhysicsComponent*,
	AliveComponent*, TextureComponent*, RectangleComponent*, Rectangle, uint32 eid, Bullet bullet);
