#pragma once
#include "Component.h"

struct MarchValues {
	int range;
	float distance;
	int facing;
	bool aggrod;
};

struct AIComponent : public Component {
        MarchValues marchValues[Constants::MaxEntities_]; //saved by id rather than index
};

void AIComponent_Add(AIComponent* aiComponent, uint32 eid, int range, int facing);
