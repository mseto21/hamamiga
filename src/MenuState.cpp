#include <stdio.h>

#include "SDL.h"
#include "SDL_image.h"
#include "Game.h"
#include "GameState.h"
#include "MenuState.h"
#include "TextureCache.h"

MenuState MenuState::menuState;

void MenuState::initialize(Game* game) {
  char* path = "assets/menu.png";
  bGround = TextureCache_CreateTexture(path, game->renderer);
}

void MenuState::close() {

}

void MenuState::handleEvents(Game* game) {
  SDL_Event e;

  if (SDL_PollEvent(&e)) {
    switch(e.type) {
    case SDL_QUIT:
      Game_Close(game);
      break;
    case SDL_KEYDOWN:
      switch(e.key.keysym.sym) {
      case SDLK_ESCAPE:
	Game_Close(game);
	break;
      case SDLK_w:
	break;
      case SDLK_s:
	break;
      }
      break;
    }
  }
}

void MenuState::update(Game* game) {
  //change highlighted text
}

void MenuState::draw(Game* game) {
  SDL_Rect rquad = { 0, 0, bGround->w, bGround->h };
  /*rquad.x = 0;
  rquad.y = 0;
  rquad.w = bGround->w;
  rquad.h = bGround->h;*/
  SDL_RenderClear(game->renderer);
  SDL_RenderCopy(game->renderer, bGround->sdltexture, NULL, &rquad);
  SDL_RenderPresent(game->renderer);
}
