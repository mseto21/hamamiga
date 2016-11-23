#include "NameComponent.h"
#include <SDL_ttf.h>
#include <iostream>

const char* FontPath_ = "assets/fonts/newcomictitle.ttf";

void NameComponent_Initialize(NameComponent* nameComponent) {
	Component_Initialize(nameComponent);
	nameComponent->r = 255;
	nameComponent->g = 255;
	nameComponent->b = 255;
	nameComponent->font = TTF_OpenFont(FontPath_, 18);
}

void NameComponent_Add(NameComponent* nameComponent, uint32 eid, Texture * texture) {
	Component_Add(nameComponent, eid);
	nameComponent->textures[eid] = texture;
}

void NameComponent_Free(NameComponent* nameComponent) {
	(void) nameComponent;
	// TO-DO: Figure out why this seg faults.
	//TTF_CloseFont(nameComponent->font);
}