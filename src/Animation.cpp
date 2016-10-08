#include "Animation.h"
#include <SDL.h>
#include <iostream>
struct SDL_Rect;

void Animation_Initialize(Animation* animation, uint16 frames, float frameTime, int spriteW, int spriteH) {
	animation->currentFrame = 0;
	animation->currentFrameTime = 0.f;
	animation->frameTime =  frameTime;
	animation->frames = frames;
	animation->spriteW = spriteW;
	animation->spriteH = spriteH;
}