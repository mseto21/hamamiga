#pragma once
#include "Types.h"

struct Texture;

struct Hat {
  String128 name;
  String128 effect;
  uint8 id;
  int hatType;
  uint32 eid;
};