#include "InteractableComponent.h"
#include "TextureCache.h"
#include <SDL_ttf.h>

const char* FontPath_ = "assets/DINCondensedBold.ttf";

void InteractableComponent_Initialize(InteractableComponent* interactableComponent) {
	Component_Initialize(interactableComponent);
	interactableComponent->font = TTF_OpenFont(FontPath_, 28);
	interactableComponent->r = 220;
	interactableComponent->g = 220;
	interactableComponent->b = 220;
}

 void InteractableComponent_Add(InteractableComponent* interactableComponent, uint32 eid, Texture* texture, int type, int hattype) {
  	Component_Add(interactableComponent, eid);
  	interactableComponent->msgs[eid] = texture;
 	interactableComponent->types[eid] = type;
 	interactableComponent->hattypes[eid] = hattype;
	interactableComponent->canBeInteractedWith[eid] = false;
 	interactableComponent->interacted[eid] = false;
}

void InteractableComponent_Free(InteractableComponent* interactableComponent) {
	TTF_CloseFont(interactableComponent->font);
}