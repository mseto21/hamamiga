#include "Game.h"
#include "constants.h"
#include "Player.h"
#include "TextureCache.h"
#include "Enemy.h"
#include "Timer.h"
#include "Renderer.h"

#include "ComponentLib/InputComponent.h"
#include "ComponentLib/MovementComponent.h"
#include "ComponentLib/RectangleComponent.h"
#include "ComponentLib/RenderComponent.h"

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using std::cout;
using std::endl;

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


void InitializeCharacters(int players, int enemies, Character* characters) {
	for (int p = 0; p < players; p++) {
		Player player;
		memcpy(&characters[p], player, sizeof(Player));
		characters[p].texture = TextureCache_CreateTexture(PLAYER_IMG, game->renderer);
	}

	Coord2D coords[enemies] = {
    { Constants::ScreenHeight_, 0 },
    { 0, Constants::ScreenWidth_},
    { Constants::ScreenHeight_, Constants::ScreenWidth_}
  };
	for (int e = 0; e < enemies; e++) {
		Enemy enemy(coords[e]);
		memcpy(&characters[e], enemy, sizeof(Enemy));
		characters[e].texture = TextureCache_CreateTexture(ENEMY_IMG, game->renderer);
	}
}


void Game_RunLoop(Game* game) {
	SDL_Event event;
	const int players = 1;
	const int enemies = 3;
	Character characters[Constants::MaxCharacters_];
	InitializeCharacters(players, enemies, &characters);

	// TO-DO: Make this less hacky
	Player player;
	player.texture = TextureCache_CreateTexture(PLAYER_IMG, game->renderer);
	characters[4] = player;

	Uint32 currentTime = SDL_GetTicks();
	Uint32 frameTime;
	Uint32 lastTime;
	float delta;

	const float TargetFps_ = 60.f;
	const float OptimalTime_ = 1000 / TargetFps_;

	bool keysdown[Constants::NumKeys_];

	while (game->running) {
		// Calculate timestep
		lastTime = currentTime;
		currentTime = SDL_GetTicks();
		frameTime = currentTime - lastTime;
		delta = frameTime / ((float)OptimalTime_);
		if (frameTime < OptimalTime_) {
			SDL_Delay((OptimalTime_) - frameTime);
		}


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
		


	  	// Update entities
		player.Update(delta);
		for (int i = 0; i < 3; i++) {
			characters[i].Update(delta);
			characters[i].Collide(&characters);
		}


		// Render
		SDL_RenderClear(game->renderer);
		Renderer_RenderCoord(game->renderer, &player.rect, player.texture);
		for (int i = 0; i < 3; i++) {
			Renderer_RenderCoord(game->renderer, &characters[i].rect, characters[i].texture);
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