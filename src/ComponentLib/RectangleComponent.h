/*
Relies on:
Usage: Gives the entity bounds that they will use to collide.
How these collisions are handled will vary from entity to entity.
*/

#include "Component.h"

struct Rectangle {
	int x;
	int y;
	int w;
	int h;
};

bool Rectangle_Intersects(const Rectangle l, const Rectangle r) {
	return (l.x > r.x && l.x + l.w < r.x + r.w) && (l.y > r.y && l.y + l.h < r.y + r.h);
}

class RectangleComponent : public Component {
public:
	void Update(float timestep);
	void SetOn(uint32 id, int x, int y, int w, int h);
	Rectangle entityRectangles[Constants::MaxEntities_]; // Saved by ID rather than index
}