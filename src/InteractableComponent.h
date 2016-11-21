#pragma once
#include "Component.h"

struct Texture;
struct _TTF_Font;

struct InteractableComponent : public Component {
	Texture* msgs[Constants::MaxEntities_];
 	int types[Constants::MaxEntities_];
 	int hattypes[Constants::MaxEntities_];
 	bool canBeInteractedWith[Constants::MaxEntities_];
 	bool interacted[Constants::MaxEntities_];
 	Texture* txt[Constants::MaxEntities_][Constants::MaxInteractableMessages_];

 	_TTF_Font* hoverFont;
 	_TTF_Font* msgFont;
 	unsigned char r, g, b;
};

void InteractableComponent_Initialize(InteractableComponent* interactableComponent);
void InteractableComponent_Add(InteractableComponent* interactableComponent, uint32 eid, Texture* texture, int type, int hattype, Texture** txt);
void InteractableComponent_Free(InteractableComponent* interactableComponent);