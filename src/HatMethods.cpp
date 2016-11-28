#include "HatMethods.h"

const int TimeBetweenColors_ = 100;
const int ColorStep_ = 5;
int DiscoHat::r = 0;
int DiscoHat::g = 0;
int DiscoHat::b = 0;
int DiscoHat::a = 60;
int DiscoHat::elapsed = 0;
int DiscoHat::color = 0;
SDL_Rect DiscoHat::Rectangle = {0, 0, Constants::ScreenWidth_, Constants::ScreenHeight_};

void DiscoHatUpdate(int elapsed) {
	DiscoHat::elapsed += elapsed;

	if (DiscoHat::elapsed > TimeBetweenColors_) {
		DiscoHat::color += ColorStep_;
		DiscoHat::r = sin(Frequency_ * (DiscoHat::color % MaxColors_)) * 127 + 128;
		DiscoHat::g = sin(Frequency_ * (DiscoHat::color % MaxColors_) + 2) * 127 + 128;
		DiscoHat::b = sin(Frequency_ * (DiscoHat::color % MaxColors_) + 4) * 127 + 128;
		DiscoHat::elapsed = 0;
	}
}

uint8 CowboyHat::bulletCount = 0;

void CowboyHatInit() {
	CowboyHat::bulletCount = 0;
}

bool CowboyHatAdd() {
	if (CowboyHat::bulletCount < 3) {
		CowboyHat::bulletCount++;
		return true;
	} else {
		return false;
	}
}

void CowboyHatSubtract() {
	if (CowboyHat::bulletCount > 0)
		CowboyHat::bulletCount--;
}