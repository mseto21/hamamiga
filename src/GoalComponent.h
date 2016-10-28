#pragma once
#include "Component.h"

struct GoalComponent : public Component {
	bool winGoal[Constants::MaxEntities_];
};

void GoalComponent_Add(GoalComponent* goalComponent, uint32 eid);