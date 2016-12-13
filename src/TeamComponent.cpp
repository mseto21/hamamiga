#include "TeamComponent.h"

void TeamComponent_Initialize(TeamComponent* teamComponent) {
	Component_Initialize(teamComponent);
	memset(&teamComponent->team, 0, Constants::MaxEntities_);
}

void TeamComponent_Add(TeamComponent* teamComponent, uint32 eid, uint8 team) {
	Component_Add(teamComponent, eid);
	teamComponent->team[eid] = team;
}