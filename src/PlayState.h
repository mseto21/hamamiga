#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "SDL.h"
#include "GameState.h"

struct Texture;

class PlayState : public GameState {
 public:
  PlayState() { }
  void initialize(Game* game);
  void close();
  void pause();
  void resume();
  void handleEvents(Game* game);
  void update(Game* game);
  void draw(Game* game);

  static PlayState* Instance() {
    return &playState;
  }

 private:
  static PlayState playState;
  Texture* bGround;
  bool paused;

};

#endif
