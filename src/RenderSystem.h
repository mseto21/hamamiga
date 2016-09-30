// Renderer.h
// Wrapper class for the SDL_Renderer
struct SDL_Renderer;
struct SDL_Window;
struct Texture;
struct Rectangle;
struct RenderComponent;

bool Renderer_Initialize(SDL_Renderer* renderer, SDL_Window* window);
void Renderer_RenderCoord(SDL_Renderer* renderer, Rectangle* rect, Texture* texture);
void Renderer_Update(RendereComponent* renderComponent);
void Renderer_CompleteRender(SDL_Renderer* renderer);
void Renderer_Free(SDL_Renderer* renderer);