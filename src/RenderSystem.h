#pragma once

// Forward Declarations
struct SDL_Renderer;
struct SDL_Window;
struct SDL_Rect;
struct Texture;
struct Rectangle;
struct TextureComponent;
struct RectangleComponent;
struct AnimationComponent;
struct MovementComponent;
struct CameraComponent;
struct TileMap;

// --------------------------------------------------------------------
void RenderSystem_Render_xywh(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Rect* clip, Texture* texture);
void RenderSystem_RenderCoord(SDL_Renderer* renderer, Rectangle* rect, SDL_Rect* clip, Texture* texture);
void RenderSystem_Update(SDL_Renderer* renderer, float delta, TextureComponent* textureComponent, RectangleComponent* rectangleComponent,
 AnimationComponent* animationComponent, MovementComponent* movementComponent, CameraComponent* cameraComponent, TileMap* map);
void RenderSystem_CompleteRender(SDL_Renderer* renderer);
