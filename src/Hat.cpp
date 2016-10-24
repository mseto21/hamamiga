#include "Hat.h"
#include "SoundCache.h"
#include "Constants.h"

#include <stdlib.h>
#include <iostream>

Hat::Hat() {
  jump = 1;
  dmgRed = 1;
  memcpy(gname,"", sizeof(gname));
  memcpy(name,"", sizeof(name));
  memcpy(effect,"", sizeof(effect));
  memcpy(gEffect,"", sizeof(gEffect));
}

void Hat::setHatType(int type, String128 currentHat) {
  switch (type) {
    case -1:
    if (strcmp(currentHat, "disco") != 0){
      memcpy(gname, "disco", sizeof(gname));
      memcpy(gEffect, "Strobe Lights!", sizeof(gEffect));
      Mix_VolumeMusic(MIX_MAX_VOLUME/4);
      Sound_Play(SoundCache_GetSound("disco"), -1);//later add different sounds for diff hats?
      }
      break;
    case 1: //jumping hat
    if (strcmp(currentHat, "bunny") != 0){
      memcpy(name, "bunny", sizeof(name));
      memcpy(effect, "2x Jump!", sizeof(effect));
      Sound_Play(SoundCache_GetSound("hatpickup"), 0);
      jump = 1.41;
      dmgRed = 1;
    }
    break;
    case 2: //construction hat
    if (strcmp(currentHat, "construction") != 0){
      memcpy(name, "construction", sizeof(name));
      memcpy(effect, "1/2 Damage!", sizeof(effect));
      dmgRed = 2;
      jump = 1;
    }
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
