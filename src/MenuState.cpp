#include <stdio.h>

#include "SDL_image.h"
#include "Game.h"
#include "MenuState.h"
#include "PlayState.h"
#include "HighScoreState.h"
#include "TextureCache.h"
#include "constants.h"

const char* MENU_PAGE = "assets/menu.png";
const char* FONT = "assets/stocky.ttf";
TTF_Font* font;

bool play = true;
bool hs = false;

const char* playM = "Play Game";
const char* hsM = "High Scores";

MenuState MenuState::menuState;

void MenuState::initialize(Game* game) {
  bGround = TextureCache_CreateTexture(MENU_PAGE, game->renderer);
  font = TTF_OpenFont(FONT, 50);
}

void MenuState::close() {
  TTF_CloseFont(font);
}

void MenuState::pause() {
}

void MenuState::resume() {
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
	//should be an array that it runs through
	play = !play;
	hs = !hs;
	break;
      case SDLK_s:
	play = !play;
	hs = !hs;
	break;
      case SDLK_RETURN:
	if (play)
	  Game_ChangeState(game, PlayState::Instance());
	else if (hs)
	  Game_ChangeState(game, HighScoreState::Instance());
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
  SDL_Color gray = {50, 50, 50, 255};
  SDL_Color white = {255, 255, 255, 255};
  SDL_Color pColor = gray;
  SDL_Color hsColor = gray;
  if (play)
    pColor = white;
  if (hs)
    hsColor = white;

  SDL_Surface* oPlay = TTF_RenderUTF8_Blended(font, playM, pColor);
  SDL_Surface* oHS = TTF_RenderUTF8_Blended(font, hsM, hsColor);
  SDL_Texture* oPlayT = SDL_CreateTextureFromSurface(game->renderer, oPlay);
  SDL_Texture* oHST = SDL_CreateTextureFromSurface(game->renderer, oHS);
  SDL_Rect playRect;
  playRect.x =  center(Constants::ScreenWidth_, oPlay->w);
  playRect.y = center(Constants::ScreenHeight_ / 2, oPlay->h);
  playRect.w = oPlay->w;
  playRect.h = oPlay->h;
  SDL_Rect hsRect;
  hsRect.x =  center(Constants::ScreenWidth_, oHS->w);
  hsRect.y = center(Constants::ScreenHeight_, oHS->h);
  hsRect.w = oHS->w;
  hsRect.h = oHS->h;
  SDL_RenderClear(game->renderer);
  SDL_RenderCopy(game->renderer, bGround->sdltexture, NULL, &rquad);
  SDL_RenderCopy(game->renderer, oPlayT, NULL, &playRect);
  SDL_RenderCopy(game->renderer, oHST, NULL, &hsRect);
  SDL_RenderPresent(game->renderer);
}

int MenuState::center(int large, int small) {
    return large/2 - small/2;
}
