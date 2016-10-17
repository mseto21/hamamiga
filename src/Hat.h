#include "Types.h"
#ifndef HAT_H
#define HAT_H

class Hat {
 private:
  float jump;
  int dmgRed;

 public:
 	String128 name;

  /**methods*/
  float getJump();
  int getDmgRed();
  
  /** Constructors */
  Hat();
  Hat(int type);

};

#endif
