#include "Character.h"

class Collision {

public:
    static bool collision(const Character p, const Character e);
    static bool collision(Coord2D, int, int, Coord2D, int, int);
    static bool collision(const Character p, const Character *e);

};
