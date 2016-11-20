#include "AIComponent.h"

void AIComponent_Add(AIComponent* aiComponent, uint32 eid, int type) {
	aiComponent->types[eid] = (AIType)type;
}

void AIComponent_Add(AIComponent* aiComponent, uint32 eid, int type, int range, int facing) {
  Component_Add(aiComponent, eid);
  aiComponent->types[eid] = (AIType)type;
  aiComponent->marchValues[eid].range = range;
  aiComponent->marchValues[eid].facing = facing;
  aiComponent->marchValues[eid].distance = 0;
  aiComponent->marchValues[eid].aggrod = false;
}
