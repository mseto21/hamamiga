#include "Character.h"

class Collision {

 public:
  bool collision(const Character p, const Character e);
  bool collision(const Character p, const Character *e);

};
