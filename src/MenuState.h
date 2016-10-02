#indef MENUSTATE_H
#define MENUSTATE_H

#include "SDL.h"
#include "gamestate.h"

class MenuState : public GameState {
 public:
  MenuState() { }
  void initialize();
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

  SDL_Surface* bGround;
};

#endif
