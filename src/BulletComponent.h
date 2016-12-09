#include "Component.h"

struct PhysicsComponent;
struct AliveComponent;
struct TextureComponent;
struct MovementComponent;
struct RectangleComponent;
struct TeamComponent;
struct Rectangle;

const int MaxBulletLife_ = 2500;

struct Bullet {
  bool friendly = false; //friendly or enemy bullet, default false?
  bool collided; //check if bullet has died from collision
  bool left = false; //direction bullet is heading
  int life;
};

struct BulletComponent : public Component {
	Bullet bullet[Constants::MaxEntities_]; //bullet information for this bullet entity
	int type[Constants::MaxEntities_];
};

void BulletComponent_Add(BulletComponent* bulletComponent, PhysicsComponent*,
	AliveComponent*, TextureComponent*, MovementComponent*, RectangleComponent*, TeamComponent* teamComponent, 
	 Rectangle, uint32 eid, bool, bool, int);
