#pragma once
#include "Types.h"
#include <SDL.h>

struct Animation {
	uint16 frames;			/* Number of frames in animation */
	uint16 currentFrame;	/* Frame currently being rendered */
	float delta;			/* Time between frames */
	float frameTime;		/* Time for frame to update */
	float currentFrameTime; /*Current time for frame*/
	int spriteW;			/* Width of each frame */
	int spriteH;			/* Height of each frame */
	SDL_Rect animationClips[8]; /* Array of SDL_Rect clips to be animated*/
};

void Animation_Initialize(Animation* animation, uint16 frames,
 float delta, int spriteW, int spriteH);