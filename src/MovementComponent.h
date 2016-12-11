#include "Component.h"

struct MovementValues {
	float xVelocity; //current xVelocity
	float yVelocity; //current yVelocity
	float xAccel; //current xAcceleration
	float yAccel; //current yAcceleration
	float accelX; //standard x acceleration
	float accelY; //standard y acceleration
	float maxXVelocity; //maximum x velocity
	float maxYVelocity; //maximum y velocity
	bool left; //direction the entity is facing
	bool grounded;
	bool flying;
	bool drunk;
};

struct MovementComponent : public Component {
	MovementValues movementValues[Constants::MaxEntities_]; // Saved by ID rather than index
};

void MovementComponent_Add(MovementComponent* movementComponent, uint32 eid, float xVelocity, float yVelocity, float xAccel, float yAccel);
void MovementComponent_Add(MovementComponent* movementComponent, uint32 eid, float xVelocity, float yVelocity, float xAccel, float yAccel, bool flying);