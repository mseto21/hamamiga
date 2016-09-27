#include "constants.h"
#include "types.h"
class Component {
public:
	Component();
	bool 	HasIndex(uint32 eid);
	void 	SetOn(uint32 eid);
	void 	SetOff(uint32 eid);
	virtual void 	Update(float timestep);
protected:
	uint32 count;
	uint32 inUse[Constants::MaxEntities_]; 		// Index of enemies being used
	bool   indexInUse[Constants::MaxEntities_]; // Might get rid of this for a search
}