#include "Game.h"
#include "constants.h"
#include "Player.h"
#include "TextureCache.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>


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

	if (!Renderer_Initialize(game->renderer, game->window)) {
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

	return true;
}

void Game_RunLoop(Game* game) {
	SDL_Event event;

	Timer_Initialize(&game->timer); // Initialize game time
	Timer_Start(&game->timer);

	Timer frameTime;	// Create frame timer
	Timer_Initialize(&frameTime);
	int frames = 0;

	// TO-DO: Make this less hacky
	TextureCache_CreateTexture("/assets/player.png", game->renderer->renderer);
	Player player;
	player.texture = TextureCache_GetTexture("/assets/player.png");

	while (game->running) {
		// Calculate frame time
		float timestep = Timer_GetTicks(&frameTime) / 1000.f;
		Timer_Start(&frameTime);
		float avgFPS = frames / (Timer_GetTicks(&game->timer) / 1000.f);
		if (avgFPS > 2000000) {
			avgFPS = 0;
		}
		Timer_Start(&game->timer);

		// Poll input
		while (SDL_PollEvent(&event) != 0) {
			player.GetInput(&event);
			if (event.type == SDL_QUIT) {
				game->running = false;
			}
			if (event.type == SDL_KEYUP &&  event.key.keysym.sym == 
				SDLK_ESCAPE){
				game->running = false;
			}
		}

		// Update
		player.Update(timestep);

		// Render
		Renderer_RenderCoord(game->renderer, &player.position, player.texture);
		Renderer_CompleteRender(game->renderer);
		++frames;
	}
}

void Game_Close(Game* game) {
	TextureCache_Free();
	Renderer_Free(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}