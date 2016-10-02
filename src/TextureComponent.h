#include "Component.h"

// Forward Declarations
struct Texture;

//--------------------------------------------------------------------
struct TextureComponent : public Component {
	Texture* textures[Constants::MaxEntities_];
};

void TextureComponent_Add(TextureComponent* textureComponent, uint32 eid, Texture* texture);