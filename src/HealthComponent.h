
#include "Component.h"

struct HealthComponent : public Component {
	uint8 health[Constants::MaxEntities_];
};

void HealthComponent_Add(HealthComponent* healthComponent, uint32 eid, uint8 startHealth);