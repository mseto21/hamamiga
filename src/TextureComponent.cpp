#include "TextureComponent.h"

// --------------------------------------------------------------------
void TextureComponent_Add(TextureComponent* textureComponent, uint32 eid, Texture* texture) {
	textureComponent->entityArray[textureComponent->count] = eid;
	textureComponent->usedEntities[eid] = true;
	textureComponent->count++;
	textureComponent->textures[eid] = texture;
}