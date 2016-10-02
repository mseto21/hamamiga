#pragma once
#include "Component.h"

struct InputComponent : public Component {};

void InputComponent_Add(InputComponent* inputComponent, uint32 eid);