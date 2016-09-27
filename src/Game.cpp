
#include "Game.h"
#include "constants.h"
#include "Player.h"
#include "TextureCache.h"
#include "Enemy.h"
#include "Timer.h"
#include "Renderer.h"

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

const char* PLAYER_IMG = "assets/player.png";
const char* ENEMY_IMG = "assets/enemy.png";

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

	return true;
}


void RenderCoord(SDL_Renderer* renderer, Coord2D* point, Texture* texture) {
	if (!renderer) {
		return;
	}
	if (!texture) {
		return;
	}

	SDL_Rect rquad;
	rquad.x = point->x;
	rquad.y = point->y;
	rquad.w = texture->w;
	rquad.h = texture->h;
	SDL_RenderCopy(renderer, texture->sdltexture, NULL, &rquad);
}


void Game_RunLoop(Game* game) {
	SDL_Event event;

	const int MaxEnemies_ = 3;
	Enemy enemies[MaxEnemies_];
	for (int i = 0; i < MaxEnemies_; i++) {
		enemies[i].texture = TextureCache_CreateTexture(ENEMY_IMG, game->renderer);
	}

	// TO-DO: Make this less hacky
	Player player;
	player.texture = TextureCache_CreateTexture(PLAYER_IMG, game->renderer);


	Uint32 currentTime = SDL_GetTicks();
	bool keysdown[Constants::NumKeys_];

	while (game->running) {
		// Get input
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

		player.GetInput(keysdown);

		// Calculate timestep
		Uint32 newTime = SDL_GetTicks();
		Uint32 frameTime = newTime - currentTime;
	  currentTime = newTime;
	  float timestep = frameTime / 1000.0;

	  // Update entities
		player.Update(timestep);
		for (int i = 0; i < MaxEnemies_; i++) {
			enemies[i].Update(timestep);
		}

		// Render
		SDL_RenderClear(game->renderer);
		Renderer_RenderCoord(game->renderer, &player.position, player.texture);
		for (int i = 0; i < MaxEnemies_; i++) {
			Renderer_RenderCoord(game->renderer, &enemies[i].position, enemies[i].texture);
		}
		SDL_RenderPresent(game->renderer);
	}
}

void Game_Close(Game* game) {
	TextureCache_Free();
	Renderer_Free(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}