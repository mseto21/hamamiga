// Renderer.h
// Wrapper class for the SDL_Renderer
struct SDL_Renderer;
struct SDL_Window;
struct Texture;
struct Coord2D;

struct Renderer {
	SDL_Renderer* renderer;
};

bool Renderer_Initialize(Renderer* renderer, SDL_Window* window);
void Renderer_RenderCoord(Renderer* renderer, Coord2D* point, Texture* texture);
void Renderer_CompleteRender(Renderer* renderer);
void Renderer_Free(Renderer* renderer);