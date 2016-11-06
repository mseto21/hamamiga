#pragma once
#include "Constants.h"
#include <SDL.h>

enum GlamourHatId {
	GlamourHatId_None = 0,
	GlamourHatId_Disco,
	GlamourHatId_Miner,
	GlamourHatId_Beer,
};




// Disco Hat Parameters
const float Frequency_ = 0.3;
const int MaxColors_ = 32;

struct DiscoHat {
	static SDL_Rect Rectangle;
	static int elapsed;
	static int a;
	static int r;
	static int g;
	static int b;
	static int color;
};

void DiscoHatUpdate(int elapsed);