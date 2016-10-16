#pragma once
#include "Types.h"
#include "TileMap.h"

struct Zone {
	String128 name;		/* The name of the current zone. */
	TileMap tileMap;
};

void Zone_Create(const char* name, Zone* embeddedZones);