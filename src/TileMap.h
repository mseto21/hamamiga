#pragma once
#include "Constants.h"
#include "Types.h"

struct Tile {
	uint16 tid;
	bool  solid;
	bool moving;
};

struct TileMap {
	int w;
	int h;
	Tile map[Constants::MaxMapWidth_][Constants::MaxMapHeight_];
};