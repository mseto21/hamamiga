#include "RectangleComponent.h"

// Rectangles intersect with all other rectangles.
// TO-DO: Make this smarter! Not too hard, give rectangles a sense of 'space'.
void RectangleComponent::Update(float timestep) {
	for (int entityIndex = 0; entityIndex < count; entityIndex++) {
		for (int otherEntityIndex = 0; otherEntityIndex < count; otherEntityIndex++) {
			if (entityIndex == otherEntityIndex) {
				continue;
			}
			if (Rectangle_Intersects(entityRectangles[entityIndex], entityRectangles[otherEntityIndex])) {
				// Handle some sort of collision
			}
		}
	}
}

// SetOn is overloaded for Rectangle Component
// The Entity's id is used as the id of it's rectangle.
void RectangleComponent::SetOn(uint32 eid, int x, int y, int w, int h) {
	Component::SetOn(eid);
	entityRectangles[eid] = {x, y, w, h};
}