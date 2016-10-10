
#include "Component.h"

struct HealthComponent : public Component {
	int health[Constants::MaxEntities_];
};

void HealthComponent_Add(HealthComponent* healthComponent, uint32 eid, int startHealth);