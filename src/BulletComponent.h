#include "Component.h"

struct PhysicsComponent;
struct AliveComponent;
struct TextureComponent;
struct MovementComponent;
struct RectangleComponent;
struct Rectangle;

struct Bullet {
  bool friendly = false; //friendly or enemy bullet, default false?
  bool collided; //check if bullet has died from collision
  bool left = false; //direction bullet is heading
};

struct BulletComponent : public Component {
	Bullet bullet[Constants::MaxEntities_]; //bullet information for this bullet entity
};

void BulletComponent_Add(BulletComponent* bulletComponent, PhysicsComponent*,
	AliveComponent*, TextureComponent*, MovementComponent*, RectangleComponent*,
	 Rectangle, uint32 eid, bool, bool);
