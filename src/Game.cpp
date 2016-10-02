#include "Game.h"
#include "Constants.h"
#include "Timer.h"
#include "InputSystem.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "TextureCache.h"
#include "EntityCache.h"

#include "RectangleComponent.h"
#include "MovementComponent.h"
#include "TextureComponent.h"
#include "InputComponent.h"

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using std::cout;
using std::endl;

//--------------------------------------------------------------------
bool Game_Initialize(Game* game) {
	game->running = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}

	game->window = SDL_CreateWindow("Fat Hactory", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		Constants::ScreenWidth_, Constants::ScreenHeight_, SDL_WINDOW_SHOWN);
	if (!game->window ) {
		return false;
	}

	// Initialize renderer
	//RenderSystem_Initialize(game->renderer, game->window);
	game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
	if (!game->renderer) {
		std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) && imgFlags)) {
		// handle error
		return false;
	}

	if (TextureCache_GetCache() == NULL) {
		std::cerr << "Error: The texture cache failed to initialize!" << std::endl;
		return false;
	}

	if (EntityCache_GetCache() == NULL) {
		std::cerr << "Error: The entity cache failed to initialize!" << std::endl;
		return false;
	}

	// Initialize Components
	game->rectangleComponent = new RectangleComponent();
	game->movementComponent = new MovementComponent();
	game->textureComponent = new TextureComponent();
	game->inputComponent = new InputComponent();
	Component_Initialize(game->rectangleComponent);
	Component_Initialize(game->movementComponent);
	Component_Initialize(game->textureComponent);
	Component_Initialize(game->inputComponent);

	return true;
}

//--------------------------------------------------------------------
void Game_RunLoop(Game* game) {
	SDL_Event event;
	Uint32 currentTime = SDL_GetTicks();
	Uint32 frameTime;
	Uint32 lastTime;
	float delta;

	bool keysdown[Constants::NumKeys_];
	Entity* player = EntityCache_GetNewEntity();
	//game->inputComponent->SetOn(player->eid);
	RectangleComponent_Add(game->rectangleComponent, player->eid, 50, 0, 32, 32);
	Texture* playerTexture = TextureCache_CreateTexture(game->renderer, "/assets/player.png", "player");
	TextureComponent_Add(game->textureComponent, player->eid, playerTexture);

	while (game->running) {
		// Calculate timestep
		lastTime = currentTime;
		currentTime = SDL_GetTicks();
		frameTime = currentTime - lastTime;
		delta = frameTime / Constants::OptimalTime_;
		if (frameTime < Constants::OptimalTime_) {
			SDL_Delay((Constants::OptimalTime_) - frameTime);
		}

		// Poll for input
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				game->running = false;
			} if (event.type == SDL_KEYDOWN &&  event.key.keysym.sym == SDLK_ESCAPE){
				game->running = false;
			}
			if (event.type == SDL_KEYDOWN) {
				keysdown[event.key.keysym.sym] = true;
			}
			if (event.type == SDL_KEYUP) {
				keysdown[event.key.keysym.sym] = false;
			}
		}

		// Update systems
		//InputSystem_Update(keysdown, game->inputComponent, game->movementComponent);
		MovementSystem_Update(delta, game->movementComponent, game->rectangleComponent);
		RenderSystem_Update(game->renderer, game->textureComponent, game->rectangleComponent);
	}
}

//--------------------------------------------------------------------
void Game_Close(Game* game) {
	delete game->rectangleComponent;
	delete game->movementComponent;
	delete game->textureComponent;
	delete game->inputComponent;
	TextureCache_Free();
	EntityCache_Free();
	RenderSystem_Free(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}