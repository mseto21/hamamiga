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
		for (uint32 e = 0; e < Constants::MaxEntities_; e++) {
			eCache->entities[e].eid = 0;
		}
	}
	return eCache;
}

/* Returns a new entity with a new eid. */
Entity* EntityCache_GetNewEntity() {
	Entity* result = nullptr;
	if (((uint)eCache->index) < Constants::MaxEntities_) {
		eCache->entities[eCache->index].eid = eCache->index;
		result = &eCache->entities[eCache->index];
		eCache->claimed[eCache->index] = true;
		eCache->index++;
	} else {
		for (uint eIndex = 0; eIndex < Constants::MaxEntities_; eIndex++) {
			if (eCache->claimed[eIndex] == false) {
				result = &eCache->entities[eCache->index];
				eCache->claimed[eCache->index] = true;
			}
		}
	}
	if (!result) {
		std::cerr << "Error: The entity could not be initialized." << std::endl;
	}
	return result;
}

/* Returns the entity at a specific eid. */
Entity* EntityCache_GetNewEntityAtIndex(uint32 eid) {
	if (eid > Constants::MaxEntities_)
		return nullptr;
	if (eCache->entities[eid].eid != eid) 
		eCache->entities[eid].eid = eid;
	return &eCache->entities[eid];
}

/* Resets the entity array. */
void EntityCache_RemoveAll() {
	eCache->index = 0;
	for (uint32 e = 0; e < Constants::MaxEntities_; e++) {
		eCache->entities[e].eid = 0;
		eCache->claimed[eCache->index] = false;
	}
}

/* Removes this entiy from the list. */
void EntityCache_Remove(uint32 eid) {
	eCache->claimed[eid] = false;
}

/* Frees the ecache. */
void EntityCache_Free() {
	EntityCache_RemoveAll();
	free(eCache);
	eCache = nullptr;
}
