#pragma once
#include "Types.h"

// Forward Declarations
struct SDL_Renderer;
struct SDL_Window;
struct SDL_Rect;
struct Texture;
struct Rectangle;
struct TextureComponent;
struct RectangleComponent;
struct AnimationComponent;
struct BulletComponent;
struct MovementComponent;
struct CameraComponent;
struct HatComponent;
struct TileMap;
struct HealthComponent;
struct GoalComponent;
struct InteractableComponent;
struct NameComponent;
struct ComponentBag;
struct _TTF_Font;
struct Game;

struct RenderSystem {
	TextureComponent* textureComponent; 
	RectangleComponent* rectangleComponent;
	AnimationComponent* animationComponent;
	BulletComponent* bulletComponent;
	MovementComponent* movementComponent;
	CameraComponent* cameraComponent;
	HatComponent* hatComponent;
	HealthComponent* healthComponent;
	GoalComponent* goalComponent;
	InteractableComponent* interactableComponent;
	NameComponent* nameComponent;
	TileMap* map;
	_TTF_Font* defaultFont;
	ComponentBag* cBag;
};

// Game specific rendering
void RenderSystem_Initialize(RenderSystem* renderSystem, ComponentBag* cBag, TileMap* tileMap, _TTF_Font* defaultFont);
void RenderSystem_Render_xywh(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Rect* clip, Texture* texture);
void RenderSystem_RenderCoord(SDL_Renderer* renderer, Rectangle* rect, SDL_Rect* clip, Texture* texture);
void RenderSystem_Update(RenderSystem* renderSystem, SDL_Renderer* renderer, uint32 delta);
void RenderSystem_CompleteRender(SDL_Renderer* renderer);
void RenderSystem_Free(RenderSystem* renderSystem);
