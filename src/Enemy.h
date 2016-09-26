#include "Character.h"
#include <SDL.h>

class Enemy : public Character {

private:

    /** Mark a new destination for the character using a random number generator. */
    void newDestination();

public:
    SDL_Texture* texture;       
    SDL_Rect rect;
    Coord2D destination;    // The coordinate that the character moves towards
    Coord2D maxPosition;    // The maximum coordinate on the screen (inclusive)

    void move();
    /** Constructor. */
    Enemy(SDL_Texture* t, SDL_Rect r, Coord2D pos);
    Enemy();

};
