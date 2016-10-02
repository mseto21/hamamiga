#pragma once

// Forward Declarations
struct SDL_Renderer;
struct SDL_Window;
struct Texture;
struct Rectangle;
struct TextureComponent;
struct RectangleComponent;

// --------------------------------------------------------------------
bool RenderSystem_Initialize(SDL_Renderer* renderer, SDL_Window* window);
void RenderSystem_RenderCoord(SDL_Renderer* renderer, Rectangle* rect, Texture* texture);
void RenderSystem_Update(SDL_Renderer* renderer, TextureComponent* textureComponent, RectangleComponent* rectangleComponent);
void RenderSystem_CompleteRender(SDL_Renderer* renderer);
void RenderSystem_Free(SDL_Renderer* renderer);