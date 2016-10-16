#pragma once
#include "Component.h"

struct AIComponent : public Component {};

void AIComponent_Add(AIComponent* aiComponent, uint32 eid);
