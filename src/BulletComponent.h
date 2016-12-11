#pragma once
#include "Component.h"

struct ComponentBag;

const uint8 MaxBullets_ = 3;

struct BulletValues {
	uint32 bulletEids[MaxBullets_];
	bool available[MaxBullets_];
	int availableBullets;
};

struct BulletComponent : public Component {
	BulletValues bulletValues[Constants::MaxEntities_];
};

void BulletComponent_Add(BulletComponent* bulletComponent, uint32 eid);
void BulletComponent_Create(BulletComponent* bulletComponent, uint32 eid, ComponentBag* cBag, int type);