#include "MovementComponent.h"
#include "RectangleComponent.h"

MovementComponent::MovementComponent(RectangleComponent* rectangleComponent) {
	Component::Component();
	this->rectangleComponent = rectangleComponent;
}

void MovementComponent::SetOn(uint32 eid, float xVelocity, float yVelocity, float xAccel, float yAccel) {
	this->xVelocity = xVelocity;
	this->yVelocity = yVelocity;
	this->xAccel 	= xAccel;
	this->yAccel 	= yAccel;
}

void MovementComponent::SetOn(uint32 eid, float xVelocity, float yVelocity, int xAccel, int yAccel) {
	Component::SetOn(eid);
	movementValues[eid] = {xVelocity, yVelocity, xAccel, yAccel};
}

void MovementComponent::Update(float timestep) {
	for (int entityIndex = 0; entityIndex < count; entityIndex++) {
		if (!rectangleComponent->HasIndex(entityIndex)) {
			continue;
		}

		// Get movement values for the entity
		MovementValue moveValue = movementValues[entityArray[entityIndex]];
		float xVelocity = moveValue.xVelocity;
		float yVelocity = moveValue.yVelocity;
		float xAccel 		= moveValue.xAccel;
		float yAccel		= moveValue.yAccel;

		// Get the entity's rectangle
		Rectangle* rectangle = &rectangleComponent->entityRectangles[entityArray[entityIndex]];

		// Move the rectangle appropriately
		rectangle->x += (int)(xVelocity * timestep);
		rectangle->y += (int)(yVelocity * timestep);
	}
}