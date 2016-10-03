#include "RectangleComponent.h"

// --------------------------------------------------------------------
void RectangleComponent_Add(RectangleComponent* rectangleComponent, uint32 eid, int x, int y, int w, int h) {
	Component_Add(rectangleComponent, eid);
	rectangleComponent->entityRectangles[eid].x = x;
	rectangleComponent->entityRectangles[eid].y = y;
	rectangleComponent->entityRectangles[eid].w = w;
	rectangleComponent->entityRectangles[eid].h = h;
}