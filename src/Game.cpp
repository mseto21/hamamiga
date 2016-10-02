#include "Game.h"
#include "GameState.h"
#include "Collision.h"
#include "constants.h"
#include "Player.h"
#include "TextureCache.h"
#include "Enemy.h"
#include "Timer.h"
#include "Renderer.h"

#include <SDL_mixer.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

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

		if( Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096 ) < 0 ) {
    	std::cerr <<"SDL_mixer could not initialize! SDL_mixer Error:"
    			  << Mix_GetError() << "\n";
    }

	return true;
}


void Game_RunLoop(Game* game) {
	while (game->running) {
	  handleEvents(game);
	  update(game);
	  draw(game);
        }
}

void Game_Close(Game* game) {
    while (!game->states.empty()) {
        game->states.back()->close();
        game->states.pop_back();
    }
    TextureCache_Free();
    Renderer_Free(game->renderer);
    SDL_DestroyWindow(game->window);
    Mix_Quit();
    SDL_Quit();
}

void Game_ChangeState(Game* game, GameState* state) {
    //close current state
    if (!game->states.empty()) {
      game->states.back()->close();
      game->states.pop_back();
    }
    //store and initialize new state
    game->states.push_back(state);
    game->states.back()->initialize(game);
}

void Game_PushState(Game* game, GameState* state) {
    if (!game->states.empty()) {
      game->states.back()->pause();
    }
    game->states.push_back(state);
    game->states.back()->initialize(game);
}

void Games_PopState(Game* game) {
    if (!game->states.empty()) {
        game->states.back()->close();
        game->states.pop_back();
    }
    if (!game->states.empty()) {
        game->states.back()->resume();
    }
}

void handleEvents(Game* game) {
    game->states.back()->handleEvents(game);
}

void update(Game* game) {
    game->states.back()->update(game);
}

void draw(Game* game) {
    game->states.back()->draw(game);
}
