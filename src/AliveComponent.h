#pragma once
#include "Component.h"

struct AliveComponent : public Component {
	bool alive[Constants::MaxEntities_];
};

void AliveComponent_Add(AliveComponent* aliveComponent, uint32 eid);