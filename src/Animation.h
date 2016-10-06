#pragma once
#include "Types.h"

struct Animation {
	uint16 frames;			/* Number of frames in animation */
	uint16 currentFrame;		/* Frame currently being rendered */
	float delta;			/* Time between frames */
	float frameTime;		/* Current time for frame */
	int spriteW;			/* Width of each frame */
	int spriteH;			/* Height of each frame */
};

void Animation_Initialize(Animation* animation, uint16 frames, float delta, int spriteW, int spriteH) {
	animation->currentFrame = 0;
	animation->frameTime = 0.f;
	animation->frames = frames;
	animation->delta = delta;
	animation->spriteW = spriteW;
	animation->spriteH = spriteH;
}