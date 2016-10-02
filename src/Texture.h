#include "Types.h"

// Texture.h handles asset loading
struct SDL_Texture;
struct SDL_Renderer;
class Character;

struct Texture {
	int w;
	int h;
	SDL_Texture* sdltexture;
	String128 path;
};

bool Texture_LoadTexture(Texture* texture, SDL_Renderer* renderer, const char* path, const char* name);
bool Texture_CreateBlank(Texture* texture, SDL_Renderer* renderer);
void Texture_Free(Texture* texture);