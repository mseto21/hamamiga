#include "Component.h"
struct _TTF_Font;
struct Texture;

struct NameComponent : public Component {
	Texture* textures[Constants::MaxEntities_];
	_TTF_Font* font;
	uint8 r;
	uint8 g;
	uint8 b;
};

void NameComponent_Initialize(NameComponent* nameComponent);
void NameComponent_Add(NameComponent* nameComponent, uint32 eid, Texture * texture);
void NameComponent_Free(NameComponent* nameComponent);