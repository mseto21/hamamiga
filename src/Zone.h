#pragma once
#include "Types.h"
#include "TileMap.h"

struct _Mix_Music;
struct Texture;

const uint8 MaxSlides_ = 16;

struct Zone {
	String128 name;		/* The name of the current zone. */
	TileMap tileMap;
	int levelWidth;
	int levelHeight;
	_Mix_Music* music;
};

void Zone_Create(const char* name, Zone* embeddedZones);