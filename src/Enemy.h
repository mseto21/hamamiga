#include "Character.h"
#include <SDL.h>

class Enemy : public Character {

private:

    /** Mark a new destination for the character using a random number generator. */
    void newDestination();

public:
    SDL_Rect rect;
    Coord2D destination;    // The coordinate that the character moves towards
    Coord2D maxPosition;    // The maximum coordinate on the screen (inclusive)
    Coord2D lastPosition;   // The last position of the enemy

    /** Constructor. */
    Enemy(Coord2D pos);
    Enemy();

    void move();             // Move the enemy
    void undoMove();         // Return to the previous position
    void reverseDirection(); // Revere direction after a collision
};
