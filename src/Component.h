#pragma once
/*
Component is a generic component class used for entity components.
All other components will derive this class.
Any additional arrays needed by other components will be indexed
by entity id. For example, if entity 23 gets a rectangle component,
its rectanglular coordinates will be at index [23], rather than
its index in 'entityArray'
*/

#include "Constants.h"
#include "Types.h"

struct Component {
	uint32 	count;
	uint32 	entityArray[Constants::MaxEntities_];  // Index of enemies being used (0, count]
	bool   	usedEntities[Constants::MaxEntities_]; // TO-DO: Replace array with a search.
};

void Component_Initialize(Component* component);
bool Component_HasIndex(Component* component, uint32 eid);
void Component_Add(Component* component, uint32);
void Component_Remove(Component* component, uint32);