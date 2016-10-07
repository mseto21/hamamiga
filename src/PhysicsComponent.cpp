#include "PhysicsComponent.h"

void PhysicsComponent_Add(PhysicsComponent* physicsComponent, uint32 eid, int mass) {
	physicsComponent->physicsValues[eid].mass = mass;
}