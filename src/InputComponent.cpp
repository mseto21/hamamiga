#include "InputComponent.h"

void InputComponent_Add(InputComponent* inputComponent, uint32 eid) {
	inputComponent->entityArray[inputComponent->count] = eid;
	inputComponent->usedEntities[eid] = true;
	inputComponent->count++;
}