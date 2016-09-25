// Texture.h handles asset loading
struct SDL_Texture;
struct SDL_Renderer;

struct Texture {
	const char* path;
	int w;
	int h;
	SDL_Texture* sdltexture;
};

bool Texture_LoadTexture(Texture* texture, const char* path, SDL_Renderer* renderer);
bool Texture_CreateBlank(Texture* texture, SDL_Renderer* renderer);
void Texture_Free(Texture* texture);