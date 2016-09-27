#include "Component.h"

struct MovementValues {
	float xVelocity;
	float yVelocity;
	float xAccel;
	float yAccel;
};

class MovementComponent : public Component {
public:
	MovementComponent();
private:
	MovementValues movementValues[Constants::MaxEntities_];
}