#include "InteractableComponent.h"
#include "TextureCache.h"
#include <SDL_ttf.h>

#include <iostream>
const char* MsgFontPath_ = "assets/fonts/The_Quest_of_Discoveries.ttf";
const char* HoverFontPath_ = "assets/fonts/Coffee_with_Sugar.ttf";

void InteractableComponent_Initialize(InteractableComponent* interactableComponent) {
	Component_Initialize(interactableComponent);
	interactableComponent->msgFont = TTF_OpenFont(MsgFontPath_, 24);
	interactableComponent->hoverFont = TTF_OpenFont(HoverFontPath_, 28);
	
	interactableComponent->r = 220;
	interactableComponent->g = 220;
	interactableComponent->b = 220;
}

 void InteractableComponent_Add(InteractableComponent* interactableComponent, uint32 eid, Texture* texture, int type, int hattype, Texture** txt) {
  	Component_Add(interactableComponent, eid);
  	interactableComponent->msgs[eid] = texture;
 	interactableComponent->types[eid] = type;
 	interactableComponent->hattypes[eid] = hattype;
	interactableComponent->canBeInteractedWith[eid] = false;
 	interactableComponent->interacted[eid] = false;

 	for (int index = 0; index < Constants::MaxInteractableMessages_; index++) {
 		interactableComponent->txt[eid][index] = txt[index];
 	}
}

void InteractableComponent_Free(InteractableComponent* interactableComponent) {
	TTF_CloseFont(interactableComponent->msgFont);
}