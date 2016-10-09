#include "PhysicsSystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "PhysicsComponent.h"

#include <SDL.h>
#include <iostream>

void PhysicsSystem_Update(PhysicsComponent* physicsComponent, MovementComponent* movementComponent) {
	for (uint32 entityIndex = 0; entityIndex < physicsComponent->count; entityIndex++) {
		if (!Component_HasIndex(movementComponent, entityIndex)) {
			continue;
		}
		// Get the movement value
		MovementValues* moveValues = &movementComponent->movementValues[movementComponent->entityArray[entityIndex]];
		moveValues->yVelocity += Constants::Gravity_;
	}
}
