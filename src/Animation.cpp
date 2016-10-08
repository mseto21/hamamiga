#include "Animation.h"
#include <SDL.h>
#include <iostream>
struct SDL_Rect;

void Animation_Initialize(Animation* animation, uint16 frames, float delta, int spriteW, int spriteH) {
	animation->currentFrame = 0;
	animation->currentFrameTime = 0.f;
	animation->frameTime =  1.f;//10.f; 
	animation->frames = frames;
	animation->delta = delta;
	animation->spriteW = spriteW;
	animation->spriteH = spriteH;

for (int i = 0; i < frames; i++){
  if (i != 0){
    animation->animationClips[i].x = animation->animationClips[i-1].x + spriteW;
  } else {
    animation->animationClips[i].x = 0;
  }
  animation->animationClips[i].y = 0;
  animation->animationClips[i].w = spriteW;
  animation->animationClips[i].h = spriteH;
  }

}