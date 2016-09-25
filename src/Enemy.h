#include "Coord2D.h"

class Enemy {

private:

    /** Mark a new destination for the character using a random number generator. */
    void newDestination();

public:
    
    Coord2D destination;    // The coordinate that the character moves towards
    Coord2D position;       // Current position of the character
    Coord2D maxPosition;    // The maximum coordinate on the screen (inclusive)

    /** Constructor. */
    Enemy();

    /** Move the character towards it's destination. */
    void move();
};