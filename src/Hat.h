#include "Types.h"
#ifndef HAT_H
#define HAT_H

class Hat {
 private:
  float jump;
  int dmgRed;

 public:
 	String128 name;
	String128 gname;
	String128 effect;
	String128 gEffect;

  /**methods*/
  void setHatType(int type, String128 currentHat);
  float getJump();
  int getDmgRed();
  
  /** Constructors */
  Hat();

};

#endif
