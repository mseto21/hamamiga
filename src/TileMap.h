#pragma once
#include "Constants.h"
#include "Types.h"

struct Tile {
	uint16 tid;
	uint8  parameters;
};

struct TileMap {
	Tile map[Constants::MaxMapWidth_][Constants::MaxMapHeight_];
};