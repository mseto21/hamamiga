#include "HealthComponent.h"
#include <iostream>

const uint32 TimeToUpdate = 500;

void HealthComponent_Add(HealthComponent* healthComponent, uint32 eid, int startHealth) {
	Component_Add(healthComponent, eid);
	healthComponent->health[eid] = startHealth;
	healthComponent->maxHealth[eid] = startHealth;
	healthComponent->invincible[eid] = 0;
	healthComponent->damageReduction[eid] = 1;

	healthComponent->startHealth[eid] = startHealth;
	healthComponent->elapsed[eid] = 0;
}

int HealthComponent_Lerp(HealthComponent* healthComponent, uint32 eid, uint32 elapsed) {
	if (healthComponent->startHealth[eid] == healthComponent->health[eid])
		return healthComponent->health[eid];
	healthComponent->elapsed[eid] += elapsed;
	if (healthComponent->health[eid] < 0) {
	  healthComponent->health[eid] = 0;
	}
	float t = (float)healthComponent->elapsed[eid] / TimeToUpdate;
	int lerp =  (1 - t) * (healthComponent->startHealth[eid]) + t * healthComponent->health[eid];
	if (lerp <= healthComponent->health[eid])  {
		healthComponent->elapsed[eid] = 0;
		healthComponent->startHealth[eid] = healthComponent->health[eid];
	}
	return lerp;
}
