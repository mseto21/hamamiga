#include "Coord2D.h"

class Character {
  
 public:
  void move();

  Coord2D position; //x-y coordinates of player
  int width; //width of sprite
  int height; //height of sprite
  double vx; //velocity x
  double vy; //velocity y
  double ax; //acceleration x
  double ay; //acceleration y
};
