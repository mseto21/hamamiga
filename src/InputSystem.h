#pragma once
/* 
Relies on: MovementComponent.
Usage: If the InputComponent is attached to an entity, the etity will 
accept basic input controls from the user.
*/

// Forward Declarations
struct MovementComponent;
struct InputComponent;
struct RectangleComponent;

void InputSystem_Update(bool keysPressed[], InputComponent* inputComponent, MovementComponent* movementComponent, RectangleComponent* rectangleComponent);