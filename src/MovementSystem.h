#pragma once

// Forward Declarations
struct MovementComponent;
struct RectangleComponent;

// --------------------------------------------------------------------
void MovementSystem_Update(float timestep, MovementComponent* movementComponent, RectangleComponent * rectangleComponent);