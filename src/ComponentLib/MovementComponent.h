/*
Relies on: RectangleComponent
Usage: Moves the entity using the specified values.
*/

#include "Component.h"

// Forward Declarations
class RectangleComponent;

struct MovementValues {
	float xVelocity;
	float yVelocity;
	float xAccel;
	float yAccel;
};

class MovementComponent : public Component {
public:
	MovementComponent(RectangleComponent* rectangleComponent);
	void SetOn(uint32 eid, float xVelocity, float yVelocity, int xAccel, int yAccel);
	void Update(float timestep);
	MovementValues movementValues[Constants::MaxEntities_]; // Saved by ID rather than index
private:
	RectangleComponent* rectangleComponent;
}