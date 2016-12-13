#include "AliveComponent.h"

void AliveComponent_Add(AliveComponent* aliveComponent, uint32 eid) {
	Component_Add(aliveComponent, eid);
	aliveComponent->alive[eid] = true;
	aliveComponent->timeAlive[eid] = 0;
}