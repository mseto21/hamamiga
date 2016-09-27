#include "Character.h"
#include <SDL.h>

struct Texture;

class Enemy : public Character {

private:

    /** Mark a new destination for the character using a random number generator. */
    void newDestination();

public:
    Texture* texture;       
    Coord2D destination;    // The coordinate that the character moves towards
    Coord2D maxPosition;    // The maximum coordinate on the screen (inclusive)

    void update(float timestep);
    void undoMove();         // Return to the previous position
    void reverseDirection(); // Revere direction after a collision

    /** Constructor. */
    Enemy(Coord2D pos);
    Enemy();

};
