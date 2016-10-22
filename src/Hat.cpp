#include "Hat.h"

Hat::Hat() {
  jump = 1;
  dmgRed = 1;
}

void Hat::setHatType(int type) {
  switch (type) {
    case -1:
      memcpy(gname, "strobe", sizeof(gname));
      break;
    case 1: //jumping hat
      memcpy(name, "bunny", sizeof(name));
      jump = 1.41;
      dmgRed = 1;
      break;
    case 2: //construction hat
      memcpy(name, "construction", sizeof(name));
      dmgRed = 2;
      jump = 1;
      break;
    default:
      break;
  }
}

float Hat::getJump() {
  return jump;
}

int Hat::getDmgRed() {
  return dmgRed;
}
