#pragma once
#include "Constants.h"
#include "Types.h"
#include <SDL.h>
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