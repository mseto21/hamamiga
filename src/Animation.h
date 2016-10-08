#pragma once
#include "Types.h"

struct SDL_Rect;

struct Animation {
	uint16 frames;			/* Number of frames in animation */
	uint16 currentFrame;	/* Frame currently being rendered */
	float delta;			/* Time between frames */
	float frameTime;		/* Current time for frame */
	int spriteW;			/* Width of each frame */
	int spriteH;			/* Height of each frame */
	SDL_Rect *animationClips; /* Array of SDL_Rect clips to be animated*/
};

void Animation_Initialize(Animation* animation, uint16 frames,
 float delta, int spriteW, int spriteH);