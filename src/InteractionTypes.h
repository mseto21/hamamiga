#pragma once
#include "Types.h"
struct ComponentBag;

enum InteractionTypes {
	InteractionType_Hat = 0,
	InteractionTypes_Dialogue,
	InteractionTypes_Cutscene,
	InteractionTypes_None
};

void ApplyHatInteraction(int hatType, uint32 eid, ComponentBag* cBag);
void PlayEventInteraction(uint32 eid, ComponentBag* cBag);