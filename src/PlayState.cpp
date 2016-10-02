#include <stdio.h>

#include "SDL.h"
#include "SDL_image.h"
#include "GameState.h"
#include "Game.h"
#include "PlayState.h"
#include "MenuState.h"

PlayState PlayState::playState;

void PlayState::initialize(Game* game) {
  //char* path = "assets/background.png"
  //bGround = TextureCache_CreateTexture(paht, game->renderer);
}

void PlayState::close() {

}

void PlayState::pause() {
  paused = true;;
}

void PlayState::resume() {
  paused = false;
}

void PlayState::handleEvents(Game* game) {
  SDL_Event e;
  if (SDL_PollEvent(&e)) {
    switch(e.type) {
    case SDL_QUIT:
      Game_Close(game);
      break;
    case: SDL_KEYDOWN:
      switch(e.key.keysym.sym) {
      case SDLK_M:
	game->changeState(MenuState::Instance());
	break;
      case SDLK_P:
	paused = !paused;
	break;
      case SDLK_p:
	paused = !paused;
      case SDLK_ESCAPE:
	Game_Close(game);
	break;
      }
      break;
    }
  }
}

void PlayState::update(Game* game) {

}

void PlayState::Draw(Game* game) {

}
