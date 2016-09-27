#ifndef Texture_H_
#define Texture_H_

// Texture.h handles asset loading
struct SDL_Texture;
struct SDL_Renderer;
class Character;

struct Texture {
	const char* path;
	int w;
	int h;
	SDL_Texture* sdltexture;
	Character* owner;
};

bool Texture_LoadTexture(Texture* texture, const char* path, SDL_Renderer* renderer, Character* owner);
bool Texture_CreateBlank(Texture* texture, SDL_Renderer* renderer);
void Texture_Free(Texture* texture);

#endif