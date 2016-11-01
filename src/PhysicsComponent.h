#pragma once
#include "Component.h"

struct PhysicsValues {
	int mass;
};

struct PhysicsComponent : public Component {
	PhysicsValues physicsValues[Constants::MaxEntities_];
};

void PhysicsComponent_Add(PhysicsComponent* physicsComponent, uint32 eid, int mass);