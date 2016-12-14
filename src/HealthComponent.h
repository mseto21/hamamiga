
#include "Component.h"

struct HealthComponent : public Component {
	int health[Constants::MaxEntities_];
	int maxHealth[Constants::MaxEntities_];
	int damageReduction[Constants::MaxEntities_];
	int invincible[Constants::MaxEntities_];

	uint32 elapsed[Constants::MaxEntities_];
	int startHealth[Constants::MaxEntities_];
};

void HealthComponent_Add(HealthComponent* healthComponent, uint32 eid, int startHealth);
int HealthComponent_Lerp(HealthComponent* healthComponent, uint32 eid, uint32 elapsed);