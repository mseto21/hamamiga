#include "Collision.h"

  /**
   * Collision assuming player and enemy are rectangles
   */
bool Collision::collision(const Character p, const Character e) {
    //checks if players x coordinate is between enemy's x coordinates
    //or if x + width is between enemy's y coordinates
    if ((p.position.x >= e.position.x &&
	 p.position.x <= (e.position.x + e.width)) ||
	((p.position.x + p.width) >= e.position.x &&
	 (p.position.x + p.width) <= (e.position.x + e.width))) {
      //checks if players y coordinate is between enemy's y coordinates
      //or if y + height is between eneym's y coordinates
      if ((p.position.y >= e.position.y &&
	   p.position.y <= (e.position.y + e.height)) ||
	  ((p.position.y + p.height) >= e.position.y &&
	   (p.position.y + p.height) <= (e.position.y + e.height))) {
	return true;
      }
    }
    return false;
  }

  /**
   * Detects collision between player and one of an 
   * array of enemies
   */
bool Collision::collision(const Character p, const Character *e) {
    int bound = (int)(sizeof(e)/sizeof(*e));
    for (int i = 0; i < bound; i++) {
      if (collision(p, e[i])) {
	return true;
      }
    }
    return false;
  }

  
