#include "Component.h"
#include <iostream>

void Component_Initialize(Component* component) {
	component->count = 0;
}

bool Component_HasIndex(Component* component, uint32 eid) {
	return component->usedEntities[eid];
}

void Component_Add(Component* component, uint32 eid) {
	if (component->count >= Constants::MaxEntities_) {
		std::cerr << "Error: Attempting to add too many entities to the component!" << std::endl;
		return;
	}
	if (eid >= Constants::MaxEntities_) {
		std::cerr << "Error: Attempting to add too many entities to the component!" << std::endl;
		return;
	}
	component->entityArray[component->count] = eid;
	component->usedEntities[eid] = true;
	component->count++;
}

void Component_Remove(Component* component, uint32 eid) {
	for (uint32 entityIndex = 0; entityIndex < component->count; entityIndex++) {
		if (component->entityArray[entityIndex] == eid) {
			component->usedEntities[eid] = false;
			return;
		}
	}
}