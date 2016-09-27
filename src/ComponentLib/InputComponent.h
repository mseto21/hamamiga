// The InputComponent itself relies on the MovementComponent.
// Other types of inputs may be available in the future.

#include "Component.h"

struct MovementComponent;

class InputComponent : public Component {
public:
	InputComponent(MovementComponent* movementComponent);
	void Update(float timestep, bool* keysPressed);
private:
	MovementComponent* movementComponent;
}