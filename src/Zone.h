#pragma once
#include "Types.h"
#include "TileMap.h"

struct _Mix_Music;
struct Texture;

const uint8 MaxSlides_ = 16;

struct Zone {
	String128 name;		/* The name of the current zone. */
	TileMap tileMap;	/* Map of zone. */
	int levelWidth;		/* Level width. */
	int levelHeight;	/* Level height. */
	_Mix_Music* music;	/* Music playing in level. */
};