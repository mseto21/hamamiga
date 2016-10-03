#include "MovementComponent.h"

void MovementComponent_Add(MovementComponent* movementComponent, uint32 eid, float xVelocity, float yVelocity, int xAccel, int yAccel) {
	Component_Add(movementComponent, eid);
	movementComponent->movementValues[eid].xVelocity 	= xVelocity;
	movementComponent->movementValues[eid].yVelocity 	= yVelocity;
	movementComponent->movementValues[eid].xAccel 		= xAccel;
	movementComponent->movementValues[eid].yAccel 		= yAccel;
}