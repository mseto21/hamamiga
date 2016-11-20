#include "AIComponent.h"

void AIComponent_Add(AIComponent* aiComponent, uint32 eid, int range, int facing) {
  Component_Add(aiComponent, eid);
  aiComponent->marchValues[eid].range = range;
  aiComponent->marchValues[eid].facing = facing;
  aiComponent->marchValues[eid].distance = 0;
  aiComponent->marchValues[eid].aggrod = false;
}
