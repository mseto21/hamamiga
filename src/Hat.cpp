#include "Hat.h"


Hat::Hat(int type) {
  Hat();
  switch (type) {
  case 0: //jumping hat
    jump = 2;
    break;
  default:
    break;
  }
}

Hat::Hat() {
  jump = 1;
}

int Hat::getJump() {
  return jump;
}
