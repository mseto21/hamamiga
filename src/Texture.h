#include "Types.h"

// Texture.h handles asset loading
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Color;
struct _TTF_Font;

struct Texture {
	int w;
	int h;
	SDL_Texture* sdltexture;
	String128 name;
	bool isAnimation;
	int flip;

	int clipX, clipY, clipW, clipH;
};

bool Texture_LoadTexture(Texture* texture, SDL_Renderer* renderer, const char* path, const char* name);
bool Texture_LoadTexture(Texture* texture, SDL_Renderer* renderer, const char* path, const char* name, int x, int y, int w, int h);
bool Texture_CreateTextureFromFont(Texture* texture, SDL_Renderer* renderer, _TTF_Font* font, SDL_Color color, const char* message, const char* name);
bool Texture_CreateBlank(Texture* texture, SDL_Renderer* renderer);
void Texture_Free(Texture* texture);