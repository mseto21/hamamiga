#include "StatSystem.h"
#include "ComponentBag.h"
#include "HealthComponent.h"


void StatSystem_Initialize(StatSystem* statSystem, ComponentBag* cBag) {
	statSystem->healthComponent 	= cBag->healthComponent;
}


void StatSystem_Update(StatSystem* statSystem, float delta) {
	
}