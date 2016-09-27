#include "MovementComponent.h"

void MovementComponent::SetOn(uint32 eid, float xVelocity, float yVelocity, float xAccel, float yAccel) {
	this->xVelocity = xVelocity;
	this->yVelocity = yVelocity;
	this->xAccel 	= xAccel;
	this->yAccel 	= yAccel;
}