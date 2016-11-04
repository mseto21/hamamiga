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
  bool friendly = true; //friendly or enemy bullet
};


struct BulletComponent : public Component {
	Bullet bullet; //bullet information for this bullet entity
	bool activated;
};

void BulletComponent_Add(BulletComponent* bulletComponent, PhysicsComponent*,
	AliveComponent*, TextureComponent*, RectangleComponent*, Rectangle, uint32 eid);
