#include "Constants.h"
#include "Types.h"
#include "Entity.h"
#include <cstdlib>

struct EntityCache {
	Entity entities[Constants::MaxEntities_];
	int index;
};

struct EntityCache* eCache;

/* Returns the entity cache. */
EntityCache* EntityCache_GetCache() {
	if (eCache == nullptr) {
		eCache = (EntityCache*)malloc(sizeof(EntityCache));
		eCache->index = 0;
	}
	return eCache;
}


Entity* EntityCache_GetNewEntity() {
	Entity* result = nullptr;
	if (((uint)eCache->index) < Constants::MaxEntities_) {
		eCache->entities[eCache->index].eid = eCache->index;
		result = &eCache->entities[eCache->index];
		eCache->index++;
	}
	return result;
}

Entity* EntityCache_GetNewEntityAtIndex(uint32 eid) {
	if (eid > Constants::MaxEntities_) {
		return nullptr;
	}
	if (eCache->entities[eid].eid != eid) {
		eCache->entities[eid].eid = eid;
		eCache->index++;
	}
	return &eCache->entities[eid];
}

void EntityCache_Free() {
	free(eCache);
}