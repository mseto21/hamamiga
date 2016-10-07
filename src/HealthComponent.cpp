#include "HealthComponent.h"

void HealthComponent_Add(HealthComponent* healthComponent, uint32 eid, uint8 startHealth) {
	Component_Add(healthComponent, eid);
	healthComponent->health[eid] = startHealth;
}