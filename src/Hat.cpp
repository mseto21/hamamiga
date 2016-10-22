#include "Hat.h"

Hat::Hat() {
  jump = 1;
  dmgRed = 1;
  memcpy(gname,"", sizeof(gname));
  memcpy(name,"", sizeof(name));
  memcpy(effect,"", sizeof(effect));
  memcpy(gEffect,"", sizeof(gEffect));
}

void Hat::setHatType(int type) {
  switch (type) {
    case -1:
      memcpy(gname, "disco", sizeof(gname));
      memcpy(gEffect, "Strobe Lights!", sizeof(gEffect));
      break;
    case 1: //jumping hat
      memcpy(name, "bunny", sizeof(name));
      memcpy(effect, "2x Jump!", sizeof(effect));
      jump = 1.41;
      dmgRed = 1;
      break;
    case 2: //construction hat
      memcpy(name, "construction", sizeof(name));
      memcpy(effect, "1/2 Damage!", sizeof(effect));
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
