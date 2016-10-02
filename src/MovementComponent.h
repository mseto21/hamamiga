#include "Component.h"

struct MovementValues {
	float xVelocity;
	float yVelocity;
	float xAccel;
	float yAccel;
};

struct MovementComponent : public Component {
	MovementValues movementValues[Constants::MaxEntities_]; // Saved by ID rather than index
};

void MovementComponent_Add(MovementComponent* movementComponent, uint32 eid, float xVelocity, float yVelocity, int xAccel, int yAccel);