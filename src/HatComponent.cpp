#include "HatComponent.h"


void HatComponent_Add(HatComponent* hatComponent, uint32 eid, Hat hat, Hat gHat) {
    Component_Add(hatComponent, eid);
	hatComponent->hats[eid].hat = hat;
	hatComponent->hats[eid].gHat = gHat;
}
