#include "FAIComponent.h"

void FAIComponent_Add(FAIComponent* faiComponent, uint32 eid, int range, int facing) {
  Component_Add(faiComponent, eid);
  faiComponent->flyValues[eid].range = range;
  faiComponent->flyValues[eid].facing = facing;
  faiComponent->flyValues[eid].distance = 0;
}
