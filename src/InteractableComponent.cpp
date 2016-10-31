#include "InteractableComponent.h"

void InteractableComponent_Add(InteractableComponent* interactableComponent, uint32 eid, const char* message, int type, int hattype) {
	Component_Add(interactableComponent, eid);
	memcpy(&interactableComponent->interactionValues[eid].messages, message, sizeof(interactableComponent->interactionValues[eid].messages));
	interactableComponent->interactionValues[eid].types = type;
	interactableComponent->interactionValues[eid].hattypes = hattype;
	interactableComponent->interactionValues[eid].canBeInteractedWith = false;
	interactableComponent->interactionValues[eid].interacted = false;
}
