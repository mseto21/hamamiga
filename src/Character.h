#ifndef Character_H_
#define Character_H_


#include "Coord2D.h"
union SDL_Event;

class Character {
  
 public:

  Character() {};
  virtual void GetInput(SDL_Event* event) {(void)event; return;};
  virtual void Update(float timestep) {(void)timestep; return;};

  Coord2D position; //x-y coordinates of player
  int width; //width of sprite
  int height; //height of sprite
  double vx; //velocity x
  double vy; //velocity y
  double ax; //acceleration x
  double ay; //acceleration y
};

#endif
