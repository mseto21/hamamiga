#pragma once
#include "Component.h"

struct InteractableComponent : public Component {
	String128 messages[Constants::MaxEntities_];
 	int types[Constants::MaxEntities_];
 	int hattypes[Constants::MaxEntities_];
 	bool canBeInteractedWith[Constants::MaxEntities_];
 	bool interacted[Constants::MaxEntities_];
};

void InteractableComponent_Add(InteractableComponent* interactableComponent, uint32 eid, const char* message, int type, int hattype);