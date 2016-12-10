#include "InteractableComponent.h"
#include "TextureCache.h"
#include <SDL_ttf.h>

#include <iostream>
const char* MsgFontPath_ = "assets/fonts/Gotham-Bold.otf";
const char* HoverFontPath_ = "assets/fonts/Coffee_with_Sugar.ttf";

void InteractableComponent_Initialize(InteractableComponent* interactableComponent) {
	Component_Initialize(interactableComponent);
	interactableComponent->msgFont = TTF_OpenFont(MsgFontPath_, 20);
	interactableComponent->hoverFont = TTF_OpenFont(HoverFontPath_, 28);
	
	interactableComponent->r = 102;
	interactableComponent->g = 102;
	interactableComponent->b = 102;
}

 void InteractableComponent_Add(InteractableComponent* interactableComponent, uint32 eid, Texture* texture, int type, int datafield, Texture** txt) {
  	Component_Add(interactableComponent, eid);
  	interactableComponent->msgs[eid] = texture;
 	interactableComponent->types[eid] = type;
 	interactableComponent->datafield[eid] = datafield;
	interactableComponent->canBeInteractedWith[eid] = false;
 	interactableComponent->interacted[eid] = false;

 	for (int index = 0; index < Constants::MaxInteractableMessages_; index++) {
 		interactableComponent->txt[eid][index] = txt[index];
 	}
}

void InteractableComponent_Free(InteractableComponent* interactableComponent) {
	TTF_CloseFont(interactableComponent->msgFont);
}