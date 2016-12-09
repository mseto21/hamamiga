#include "TeamComponent.h"

void TeamComponent_Initialize(TeamComponent* teamComponent) {
	Component_Initialize(teamComponent);
}

void TeamComponent_Add(TeamComponent* teamComponent, uint32 eid, uint8 team) {
	Component_Add(teamComponent, eid);
	teamComponent->team[eid] = team;
}