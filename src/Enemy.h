#include "Character.h"
#include <SDL.h>

struct Texture;

class Enemy : public Character {

private:

    /** Mark a new destination for the character using a random number generator. */
    void newDestination();

public:
    Texture* texture;       
    SDL_Rect rect;
    Coord2D destination;    // The coordinate that the character moves towards
    Coord2D maxPosition;    // The maximum coordinate on the screen (inclusive)

    void Update(float imestep);
    /** Constructor. */
    Enemy(SDL_Rect r, Coord2D pos);
    Enemy();

};
