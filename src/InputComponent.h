#pragma once
#include "Component.h"

struct InputComponent : public Component {
	bool interact[Constants::MaxEntities_];
};

void InputComponent_Add(InputComponent* inputComponent, uint32 eid);