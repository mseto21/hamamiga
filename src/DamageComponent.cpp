#include "DamageComponent.h"

void DamageComponent_Add(DamageComponent* damageComponent, uint32 eid, int damage) {
  Component_Add(damageComponent, eid);
  damageComponent->damageValues[eid].damage = damage;
}
