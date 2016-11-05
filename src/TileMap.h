#pragma once
#include "Constants.h"
#include "Types.h"
#include "Rectangle.h"

struct Tile {
	uint16 tid[Constants::MaxTileAnimations_];
	bool solid;
	bool winning;

	float speed;
	int animationTime;
	int elapsed;
	int currentIndex;

	Rectangle rectangle;
};

struct TileMap {
	int w;
	int h;
	Tile map[Constants::MaxMapWidth_][Constants::MaxMapHeight_];
};