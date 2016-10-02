#include "RectangleComponent.h"

// --------------------------------------------------------------------
void RectangleComponent_Add(RectangleComponent* rectangleComponent, uint32 eid, int x, int y, int w, int h) {
	rectangleComponent->entityArray[rectangleComponent->count] = eid;
	rectangleComponent->usedEntities[eid] = true;
	rectangleComponent->count++;
	rectangleComponent->entityRectangles[eid].x = x;
	rectangleComponent->entityRectangles[eid].y = y;
	rectangleComponent->entityRectangles[eid].w = w;
	rectangleComponent->entityRectangles[eid].h = h;
}