#include "Component.h"

struct PhysicsComponent;
struct AliveComponent;
struct TextureComponent;
struct RectangleComponent;
struct Rectangle;

struct Vector2
{
  float x = 0;
  float y = 0;
};

struct Bullet {
	Vector2 position; //figure out how to use this with movement system?
  Vector2 velocity; //velocity not used right now
  bool friendly = false; //friendly or enemy bullet, default false?
  bool collided; //check if bullet has died from collision
};


struct BulletComponent : public Component {
	Bullet bullet[Constants::MaxEntities_]; //bullet information for this bullet entity
	bool activated;
	//bool alive[Constants::MaxEntities_];
};

void BulletComponent_Add(BulletComponent* bulletComponent, PhysicsComponent*,
	AliveComponent*, TextureComponent*, RectangleComponent*, Rectangle, uint32 eid, bool);
