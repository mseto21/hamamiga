#include "TextureComponent.h"

// --------------------------------------------------------------------
void TextureComponent_Add(TextureComponent* textureComponent, uint32 eid, Texture* texture) {
	Component_Add(textureComponent, eid);
	textureComponent->textures[eid] = texture;
}