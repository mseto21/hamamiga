#include "HatComponent.h"
#include "Interactions.h"


void HatComponent_Add(HatComponent* hatComponent, uint32 eid, Hat hat, Hat gHat) {
    Component_Add(hatComponent, eid);
	hatComponent->hats[eid].hat = hat;
	hatComponent->hats[eid].gHat = gHat;
	hatComponent->hats[eid].hat.id = -1;
  	hatComponent->hats[eid].hat.hatType = HatTypes_Empty;
    hatComponent->hats[eid].hat.eid = -1;
    memcpy(&hatComponent->hats[eid].hat.name, "null", sizeof(String128));
    memcpy(&hatComponent->hats[eid].hat.effect, "null", sizeof(String128));

    hatComponent->hats[eid].gHat.id = -1;
  	hatComponent->hats[eid].gHat.hatType = HatTypes_Empty;
    hatComponent->hats[eid].gHat.eid = -1;
    memcpy(&hatComponent->hats[eid].gHat.name, "null", sizeof(String128));
    memcpy(&hatComponent->hats[eid].gHat.effect, "null", sizeof(String128));
}
