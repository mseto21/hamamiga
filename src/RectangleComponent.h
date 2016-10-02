#include "Component.h"
#include "Rectangle.h"

struct RectangleComponent : public Component {
	Rectangle entityRectangles[Constants::MaxEntities_]; // Saved by ID rather than index
};

void RectangleComponent_Add(RectangleComponent* rectangleComponent, uint32 id, int x, int y, int w, int h);