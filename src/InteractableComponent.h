#pragma once
#include "Component.h"

struct InteractionValues {
	String128 messages;
	int types;
	int hattypes;
	bool canBeInteractedWith;
	bool interacted;
};

struct InteractableComponent : public Component {
	InteractionValues interactionValues[Constants::MaxEntities_];
};

void InteractableComponent_Add(InteractableComponent* interactableComponent, uint32 eid, const char* message, int type, int hattype);