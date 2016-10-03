#ifndef HIGHSCORESTATE_H
#define HIGHSCORESTATE_H

#include "SDL.h"
#include "GameState.h"

struct Texture;

class HighScoreState : public GameState {
 public:
  HighScoreState() { }
  void initialize(Game* game);
  void close();
  void pause();
  void resume();
  void handleEvents(Game* game);
  void update(Game* game);
  void draw(Game* game);
  static HighScoreState* Instance() {
    return &highScoreState;
  }

 private:
  static HighScoreState highScoreState;

  Texture* bGround;
};

#endif
