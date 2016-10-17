#include "Hat.h"


Hat::Hat(int type) {
  Hat();
  switch (type) {
    case 0: //jumping hat
      memcpy(name, "bunny", sizeof(name));
      jump = 1.5;
      break;
    case 1: //army hat
      memcpy(name, "construction", sizeof(name));
      dmgRed = 2;
      break;
    default:
      break;
  }
}

Hat::Hat() {
  jump = 1;
  dmgRed = 1;
}

float Hat::getJump() {
  return jump;
}

int Hat::getDmgRed() {
  return dmgRed;
}
