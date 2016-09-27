#include "Collision.h"
#include <iostream>
  /**
   * Collision assuming player and enemy are rectangles
   */
bool Collision::collision(const Character p, const Character e) {
    std::cout << p.texture->w << std::endl;
    //checks if players x coordinate is between enemy's x coordinates
    //or if x + width is between enemy's y coordinates
    if ((p.position.x >= e.position.x &&
    	 p.position.x <= (e.position.x + e.texture->w)) ||
    	((p.position.x + p.texture->w) >= e.position.x &&
    	 (p.position.x + p.texture->w) <= (e.position.x + e.texture->w))) {
      //checks if players y coordinate is between enemy's y coordinates
      //or if y + height is between eneym's y coordinates
      if ((p.position.y >= e.position.y &&
    	   p.position.y <= (e.position.y + e.texture->h)) ||
    	  ((p.position.y + p.texture->h) >= e.position.y &&
    	   (p.position.y + p.texture->h) <= (e.position.y + e.texture->h))) {
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

  
