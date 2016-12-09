#include "Component.h"

struct TeamComponent : public Component {
	uint8 team[Constants::MaxEntities_];
};

void TeamComponent_Initialize(TeamComponent* teamComponent);
void TeamComponent_Add(TeamComponent* teamComponent, uint32 eid, uint8 team);
