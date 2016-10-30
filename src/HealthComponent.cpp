#include "HealthComponent.h"

void HealthComponent_Add(HealthComponent* healthComponent, uint32 eid, int startHealth) {
	Component_Add(healthComponent, eid);
	healthComponent->health[eid] = startHealth;
	healthComponent->maxHealth[eid] = startHealth;
	healthComponent->invincible[eid] = false;
	healthComponent->damageReduction[eid] = 1;
}
