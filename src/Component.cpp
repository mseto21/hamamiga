#include "Component.h"
#include <iostream>

void Component_Initialize(Component* component) {
	component->count = 0;
	for (uint32 i = 0; i < Constants::MaxEntities_; i++) {
		component->usedEntities[i] = false;
		component->entityArray[i] = 0;
	}
}

bool Component_HasIndex(Component* component, uint32 eid) {
	if (!component) {
		std::cerr << "Error: The component has not yet been initialized for has_index!" << std::endl;
		return false;
	}
	if (eid >= Constants::MaxEntities_) {
		std::cerr << "Error: The largest of shenanigans!" << std::endl;
		return false;
	}
	return component->usedEntities[eid];
}

void Component_Add(Component* component, uint32 eid) {
	if (component->count >= Constants::MaxEntities_) {
		std::cerr << "Error: Attempting to add too many entities to the component!" << std::endl;
		return;
	}
	if (component->count >= Constants::MaxEntities_) {
		std::cerr << "Error: Attempting to add too many entities to the component!" << std::endl;
		return;
	}
	component->entityArray[component->count] = eid;
	component->usedEntities[eid] = true;
	component->count++;
}

void Component_Disable(Component* component, uint32 eid) {
	if (component->usedEntities[eid]) {
		component->usedEntities[eid] = false;
	}
}

void Component_ForceRemove(Component* component, uint32 eid) {
	for (uint32 entityIndex = 0; entityIndex < component->count; entityIndex++) {
		if (component->entityArray[entityIndex] == eid && component->usedEntities[eid] != false) { //and already isn't removed
			component->usedEntities[eid] = false;
			component->count--;
			component->entityArray[entityIndex] = component->entityArray[component->count];
			return;
		}
	}
}