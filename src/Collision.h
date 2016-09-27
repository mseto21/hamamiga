#include "Character.h"

class Collision {

public:
    static bool collision(const Character p, const Character e);
    static bool collision(const Character p, const Character *e);

};
