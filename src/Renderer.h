// Renderer.h
// Wrapper class for the SDL_Renderer
struct SDL_Rect;
struct SDL_Renderer;
struct SDL_Window;
struct Texture;
struct Coord2D;

bool Renderer_Initialize(SDL_Renderer* renderer, SDL_Window* window);
void Renderer_RenderCoord(SDL_Renderer* renderer, Coord2D* point, Texture* texture, SDL_Rect* clip);
void Renderer_CompleteRender(SDL_Renderer* renderer);
void Renderer_Free(SDL_Renderer* renderer);