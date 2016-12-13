#pragma once
#include "Component.h"

enum AIType {
  AIType_Marcher = 0,
  AIType_Projectile,
  AIType_Thrower,
  AIType_Flyer,
  AIType_Jumper,
  AIType_Peter,
};

struct MarchValues {
	int range;
	float distance;
	int facing;
	bool aggrod;
};

struct AIComponent : public Component {
    MarchValues marchValues[Constants::MaxEntities_];
    AIType types[Constants::MaxEntities_];
};

void AIComponent_Add(AIComponent* aiComponent, uint32 eid, int type);
void AIComponent_Add(AIComponent* aiComponent, uint32 eid, int type, int range, int facing);