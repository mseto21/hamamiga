#include "InputComponent.h"

void InputComponent_Add(InputComponent* inputComponent, uint32 eid) {
	Component_Add(inputComponent, eid);
	inputComponent->interact[eid] = false;
}