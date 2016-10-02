#include <stdio.h>

#include "SDL.h"
#include "SDL_image.h"
#include "Game.h"
#include "gamestate.h"
#include "titlestate.h"
#include "menustate.h"

TitleState TitleState::titleState;

void TitleState::initialize(Game* game) {
    char* path = "assets/title.png";
    bGround = TextureCache_CreateTexture(path, game->renderer);
    char* path2 = "assets/blackScreen.png";
    fader = TextureCache_CreateTexture(path2, game->renderer);
    alpha = 255;
}

void TitleState::close() {
    SDL_FreeSurface(bGround);
    SDL_FreeSurace(fader);
}

void TitleState::handleEvents(Game* game) {
    SDL_Event e;

    if (SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_QUIT;
                Game_Close(game);
		break;
	    case SDL_KEYDOWN:
	      switch(e.key.keysym.sym) {
	      case SDLK_ESCAPE:
		Game_Close(game);
	      case SDLK_m:
		game->changeState(MenuState::Instance());
		break;
	      case SDLK_M:
		game->changeState(MenuState::Instance());
		break;
	      }
	      break;
	}
    }
}

void TitleState::Update(Game* game) {
   alpha--;
   if (alpha < 0) {
     alpha = 0;
     count++;
   }
   if (count == 20) {
     game->changeState(MenuState::Instance());
   }
   SDL_SetTextureAlphaMod(fader->sdltexture, alpha);
}

void TitleState::Draw(Game* game) {
  SDL_Rect rquad;
  rquad.x = 0;
  rquad.y = 0;
  rquad.w = bGround->w;
  rquad.h = bGround->h;
  SDL_RenderClear(game->renderer);
  SDL_RenderCopy(game->renderer, bGround->sdltexture, NULL, rquad);
  SDL_RenderCopy(game->renderer, fader->sdltexture, NULL, rquad);
  SDL_RenderPresent(game->renderer);
}
