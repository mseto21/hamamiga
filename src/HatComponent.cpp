#include "HatComponent.h"

/*
 * Adding a jumping hat
 *
 */
void HatComponent_Add(HatComponent* hatComponent, uint32 eid, Hat hat) {
        Component_Add(hatComponent, eid);
	hatComponent->hats[eid].hat = hat;
}