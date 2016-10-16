#include "PhysicsComponent.h"

void PhysicsComponent_Add(PhysicsComponent* physicsComponent, uint32 eid, int mass) {
    Component_Add(physicsComponent, eid);
	physicsComponent->physicsValues[eid].mass = mass;
}
