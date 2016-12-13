#include "KillSystem.h"
#include "AliveComponent.h"
#include "ComponentBag.h"
#include <iostream>
void KillSystem_Initialize(KillSystem* killSystem, ComponentBag* cBag) {
	killSystem->aliveComponent 	= cBag->aliveComponent;
	killSystem->cBag 			= cBag;
}


void KillSystem_Update(KillSystem* killSystem) {
	AliveComponent* aliveComponent = killSystem->aliveComponent;
	ComponentBag* cBag = killSystem->cBag;

	for (uint32 entityIndex = 0; entityIndex < aliveComponent->count; entityIndex++) {
		uint32 eid = aliveComponent->entityArray[entityIndex];
		aliveComponent->timeAlive[eid] += Constants::OptimalTime_;
		if (!aliveComponent->alive[eid]) {
			Component_DisableEntity(cBag, eid);
		}
	}
}

void KillSystem_Free(KillSystem* killSystem) {
	killSystem->cBag = nullptr;
	killSystem->aliveComponent = nullptr;
}