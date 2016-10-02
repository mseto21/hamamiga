#include <stdio.h>

#include "SDL.h"
#include "SDL_image.h"
#include "Game.h"
#include "GameState.h"
#include "TitleState.h"
#include "MenuState.h"
#include "TextureCache.h"

TitleState TitleState::titleState;

void TitleState::initialize(Game* game) {
    char* path = "assets/title.png";
    bGround = TextureCache_CreateTexture(path, game->renderer);
    char* path2 = "assets/blackBackground.png";
    fader = TextureCache_CreateTexture(path2, game->renderer);
    alpha = 255;
}

void TitleState::close() {

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
  SDL_RenderCopy(game->renderer, fader->sdltexture, NULL, &rquad);
  SDL_RenderPresent(game->renderer);
}
