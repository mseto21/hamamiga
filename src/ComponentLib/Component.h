/*
Component is a generic component class used for entity components.
All other components will derive this class.
Any additional arrays needed by other components will be indexed
by entity id. For example, if entity 23 gets a rectangle component,
its rectanglular coordinates will be at index [23], rather than
its index in 'entityArray'
*/

#include "constants.h"
#include "types.h"

class Component {
public:
	Component();
	bool 	HasIndex(uint32 eid);
	virtual void 	SetOn(uint32 eid);
	virtual void 	SetOff(uint32 eid);
	virtual void 	Update(float timestep);
protected:
	uint32 count;
	uint32 entityArray[Constants::MaxEntities_];  // Index of enemies being used (0, count]
	bool   usedEntities[Constants::MaxEntities_]; // TO-DO: Replace array with a search.
}