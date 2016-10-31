#include "InteractableComponent.h"

void InteractableComponent_Add(InteractableComponent* interactableComponent, uint32 eid, const char* message, int type, int hattype) {
	Component_Add(interactableComponent, eid);
	memcpy(&interactableComponent->messages[eid], message, sizeof(interactableComponent->messages[eid]));
	interactableComponent->types[eid] = type;
	interactableComponent->hattypes[eid] = hattype;
	interactableComponent->canBeInteractedWith[eid] = false;
	interactableComponent->interacted[eid] = false;
}