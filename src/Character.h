#ifndef Character_H_
#define Character_H_

#include "Texture.h"
#include "Coord2D.h"

class Character {
  
 public:

  Character() {};
  virtual void GetInput(bool* KeysDown) {(void)KeysDown; return;};
  virtual void Update(float timestep) {(void)timestep; return;};

  Coord2D position; //x-y coordinates of player
  Coord2D lastPosition; //last coordinates of the 
  Texture* texture;
  double vx; //velocity x
  double vy; //velocity y
  double ax; //acceleration x
  double ay; //acceleration y
};

#endif
