#include "Hat.h"


Hat::Hat(int type) {
  switch (type) {
    case 0: //jumping hat
      memcpy(name, "bunny", sizeof(name));
      jump = 1.5;
      dmgRed = 1;
      break;
    case 1: //army hat
      memcpy(name, "construction", sizeof(name));
      dmgRed = 2;
      jump = 1;
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
