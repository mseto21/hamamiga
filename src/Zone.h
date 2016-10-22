#pragma once
#include "Types.h"
#include "TileMap.h"

struct _Mix_Music;

struct Zone {
	String128 name;		/* The name of the current zone. */
	TileMap tileMap;
	_Mix_Music* music;
};

void Zone_Create(const char* name, Zone* embeddedZones);