#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "GameState.h"

struct Texture;

class MenuState : public GameState {
 public:
  MenuState() { }
  void initialize(Game* game);
  void close();
  void pause();
  void resume();
  void handleEvents(Game* game);
  void update(Game* game);
  void draw(Game* game);
  static MenuState* Instance() {
    return &menuState;
  }

 private:
  static MenuState menuState;

  Texture* bGround;
  TTF_Font* font;

  int center(int large, int small);
};

#endif
