#include "Component.h"


Component::Component() {
	count = 0;
	memset(&indexInUse, 0, sizeof(indexInUse));
}


void Component::SetOn(int eid) {
	inUse[count] = eid;
	indexInUse[eid] = true;
	count++;
}


void Component::SetOff(int eid) {
	for (int entityIndex = 0; entityIndex < count; entityIndex++) {
		if (inUse[entityIndex] == eid) {

			// Move all entities after removed entity to the previous slot.
			for (int endEntityIndex = entityIndex; endEntityIndex < count - 1; endEntityIndex++) {
				inUse[endEntityIndex] = inUse[endEntityIndex+1];
			}
			indexInUse[eid] = false;
			count--;
			return;
		}
	}
}


bool Component::HasIndex(int eid) {
	return indexInUse[eid];
}