#pragma once

//Forward Declarations
struct MovementComponent;
struct AIComponent;
struct RectangleComponent;

void AISystem_Update(AIComponent* aiComponent, MovementComponent* movementComponent, RectangleComponent* rectangleComponent, float timeStep);
