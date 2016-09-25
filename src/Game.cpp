#include "Game.h"
#include "constants.h"
#include "Player.h"
#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

bool InitializeWindow(Game* game) {
	game->window = SDL_CreateWindow("Fat Hactory", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		Constants::ScreenWidth_, Constants::ScreenHeight_, SDL_WINDOW_SHOWN);
	if (!game->window ) {
		return false;
	}
	return true;
}

bool InitializeRenderer(Game* game) {
	game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
	if (!game->renderer) {
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	return true;
}

bool Game_Initialize(Game* game) {
	game->running = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}

	if (!InitializeWindow(game)) {
		return false;
	}

	if (!InitializeRenderer(game)) {
		return false;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) && imgFlags)) {
		// handle error
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

	Player player;

	while (game->running) {

		// Poll input
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_KEYUP){
				player.move(event.key.keysym.sym);
			}
			if (event.type == SDL_QUIT) {
				game->running = false;
			}
			if (event.type == SDL_KEYUP &&  event.key.keysym.sym == 
				SDLK_ESCAPE){
				game->running = false;
			}
		}

	//	float timestep = Timer_GetTicks(&frameTime) / 1000.f;
		Timer_Start(&frameTime);
		float avgFPS = frames / (Timer_GetTicks(&game->timer) / 1000.f);
		if (avgFPS > 2000000) {
			avgFPS = 0;
		}
		Timer_Start(&game->timer);
		++frames;
	}
}

void Game_Close(Game* game) {
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}