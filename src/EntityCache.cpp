#include "EntityCache.h"
#include "Constants.h"
#include "Types.h"
#include <cstdlib>
#include <iostream>

struct EntityCache* eCache;

/* Returns the entity cache. */
EntityCache* EntityCache_GetCache() {
	if (eCache == nullptr) {
		eCache = (EntityCache*)malloc(sizeof(EntityCache));
		eCache->index = 0;
		memset(&eCache->entities, 0, sizeof(eCache->entities));
	}
	return eCache;
}

/* Returns a new entity with a new eid. */
Entity* EntityCache_GetNewEntity() {
	Entity* result = nullptr;
	if (((uint)eCache->index) < Constants::MaxEntities_) {
		eCache->entities[eCache->index].eid = eCache->index;
		result = &eCache->entities[eCache->index];
		eCache->index++;
	}
	if (!result) {
		std::cerr << "Error: The player could not be initialized." << std::endl;
	}
	return result;
}

/* Returns the entity at a specific eid. */
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

/* Resets the entity array. */
void EntityCache_RemoveAll() {
	eCache->index = 0;
	memset(&eCache->entities, 0, sizeof(eCache->entities));
}

/* Frees the ecache. */
void EntityCache_Free() {
	std::cout << "Freeing entity cache" << std::endl;
	free(eCache);
}
