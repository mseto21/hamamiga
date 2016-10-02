#include "MovementComponent.h"

void MovementComponent_Add(MovementComponent* movementComponent, uint32 eid, float xVelocity, float yVelocity, int xAccel, int yAccel) {
	movementComponent->entityArray[movementComponent->count] = eid;
	movementComponent->usedEntities[eid] = true;
	movementComponent->count++;
	movementComponent->movementValues[eid].xVelocity 	= xVelocity;
	movementComponent->movementValues[eid].yVelocity 	= yVelocity;
	movementComponent->movementValues[eid].xAccel 		= xAccel;
	movementComponent->movementValues[eid].yAccel 		= yAccel;
}