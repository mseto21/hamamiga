#pragma once
#include "Component.h"

struct DamageValues {
  int damage;
};

struct DamageComponent:public Component {
  DamageValues damageValues[Constants::MaxEntities_];
};

void DamageComponent_Add(DamageComponent* damageComponent, uint32 eid, int damage);
