#include <stdio.h>

#include <SDL_image.h>
#include "Game.h"
#include "TitleState.h"
#include "TextureCache.h"
#include "MenuState.h"

const char* TITLE_SCREEN = "assets/title.png";
const char* FADE_IN = "assets/blackBackground.png";

TitleState TitleState::titleState;

void TitleState::initialize(Game* game) {
    bGround = TextureCache_CreateTexture(TITLE_SCREEN, game->renderer);
    fader = TextureCache_CreateTexture(FADE_IN, game->renderer);
    alpha = 255;
}

void TitleState::close() {

}

void TitleState::pause() {
}

void TitleState::resume() {
}

void TitleState::handleEvents(Game* game) {
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
	      case SDLK_m:
		Game_ChangeState(game, MenuState::Instance());
		break;
	      }
	      break;
	}
    }
}

void TitleState::update(Game* game) {
   alpha--;
   if (alpha < 0) {
     alpha = 0;
     count++;
   }
   if (count == 20) {
     Game_ChangeState(game, MenuState::Instance());
   }
   SDL_SetTextureAlphaMod(fader->sdltexture, alpha);
}

void TitleState::draw(Game* game) {
  SDL_Rect rquad;
  rquad.x = 0;
  rquad.y = 0;
  rquad.w = bGround->w;
  rquad.h = bGround->h;
  SDL_RenderClear(game->renderer);
  SDL_RenderCopy(game->renderer, bGround->sdltexture, NULL, &rquad);
  //SDL_RenderCopy(game->renderer, fader->sdltexture, NULL, &rquad);
  SDL_RenderPresent(game->renderer);
}
