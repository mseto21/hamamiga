#pragma once
#include "Constants.h"
#include "Types.h"
#include "Entity.h"

struct EntityCache {
	Entity entities[Constants::MaxEntities_];
	bool claimed[Constants::MaxEntities_];
	int index;
};

EntityCache* EntityCache_GetCache();
Entity* EntityCache_GetNewEntity();
Entity* EntityCache_GetNewEntityAtIndex(uint32 eid);
void EntityCache_RemoveAll();
void EntityCache_Remove(uint32 eid);
void EntityCache_Free();