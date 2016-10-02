#include <stdio.h>

#include "SDL.h"
#include "Game.h"
#include "gamestate.h"
#include "titlestate.h"
#include "menustate.h"

TitleState TitleState::titleState;

void TitleState::initialize() {
    SDL_Surface* temp;
    if ((temp = IMG_Load("assets/title.png") == NULL) {
        std::cerr << "Could not load title.png! Error: " << IMG_GetError() << std::endl;
    }
    bGround = SDL_DisplayFormat(temp);

    SDL_FreeSurface(temp);
    fader = SDL_CreateRGBSurface(SDL_SRCALPHA, bGround->w, bGround->h,
			   bGround->format->BitsPerPixel,
			   bGround->format->Rmask, bGround->format->Gmask,
 			   bGround->format->Bmask, bGround->format->Amask);
    SDL_FilRect(fader, NULL, SDL_MapRGB(bGround->format, 0, 0, 0));
      
    alpha = 255;
  
    SDL_SetAlpha(fader, SDL_SRCALPHA, alpha);
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
   SDL_SetAlpha(fader, SDL_SRCALPHA, alpha);
}

void TitleState::Draw(Game* game) {
  SDL_BlitSurface(bGround, NULL, game->screen, NULL);
  if (alpha != 0) {
    SDL_BlitSurface(fader, NULL, game->screen, NULL);
  }
  SDL_UpadateRect(game->screen, 0, 0, 0, 0);
}
