#ifndef HAT_H
#define HAT_H

class Hat {
 private:
  float jump;
  int dmgRed;

 public:

  /**methods*/
  float getJump();
  int getDmgRed();
  
  /** Constructors */
  Hat();
  Hat(int type);

};

#endif
