#include "Component.h"
#include "Hat.h"

struct HatCollection {
  Hat hat;
  Hat gHat;
};

struct HatComponent : public Component {
	HatCollection hats[Constants::MaxEntities_];
};

void HatComponent_Add(HatComponent* hatComponent, uint32 eid, Hat hat, Hat gHat);
