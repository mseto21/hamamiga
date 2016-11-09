#pragma once
#include "Component.h"

struct FlyValues {
	int range;
	float distance;
	int facing;
};

struct FAIComponent : public Component {
        FlyValues flyValues[Constants::MaxEntities_]; //saved by id rather than index
};

void FAIComponent_Add(FAIComponent* faiComponent, uint32 eid, int range, int facing);
