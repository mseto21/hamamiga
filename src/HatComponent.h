#include "Component.h"
#include "Hat.h"

struct HatCollection {
  Hat hat;
  //potentially add glamour hat here
};

struct HatComponent : public Component {
  HatCollection hats[Constants::MaxEntities_]; //Saved by id rather than index
};

void HatComponent_Add(HatComponent* hatComponent, uint32 eid, Hat hat);
