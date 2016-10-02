#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Game.h"

class GameState {

public:
  GameState() { }
  virtual void initialize(Game* game) = 0;
  virtual void close() = 0;
  virtual void pause() = 0;
  virtual void resume() = 0;
  virtual void handleEvents(Game* game) = 0;
  virtual void update(Game* game) = 0;
  virtual void draw(Game* game) = 0;

  void changeState(Game* game, GameState* state) {
    game->ChangeState(state);
  }
};

#endif
