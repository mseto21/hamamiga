#include "Component.h"

Component::Component() {
	count = 0;
	memset(&usedEntities, 0, sizeof(usedEntities));
}

void Component::SetOn(int eid) {
	entityArray[count] = eid;
	usedEntities[eid] = true;
	count++;
}

void Component::SetOff(int eid) {
	for (int entityIndex = 0; entityIndex < count; entityIndex++) {
		if (entityArray[entityIndex] == eid) {
			for (int endEntityIndex = entityIndex; endEntityIndex < count - 1; endEntityIndex++) {
				entityArray[endEntityIndex] = entityArray[endEntityIndex+1];
			}
			usedEntities[eid] = false;
			count--;
			return;
		}
	}
}

void Component::Update(float timestep) {
	(void) timestep;
}

bool Component::HasIndex(int eid) {
	return usedEntities[eid];
}