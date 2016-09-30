/* 
Relies on: MovementComponent.
Usage: If the InputComponent is attached to an entity, the etity will 
accept basic input controls from the user.
*/
#include "Component.h"

// Forward Declarations
class MovementComponent;

class InputComponent : public Component {
public:
	InputComponent(MovementComponent* movementComponent);
	void Update(float timestep, bool* keysPressed);
private:
	MovementComponent* movementComponent;
}