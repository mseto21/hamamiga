#ifndef HAT_H
#define HAT_H

class Hat {
 private:
  int jump;
  int dmgRed;

 public:

  /**methods*/
  int getJump();
  int getDmgRed();
  
  /** Constructors */
  Hat();
  Hat(int type);

};

#endif
