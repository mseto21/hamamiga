#include "AnimationComponent.h"

void AnimationComponent_Add(AnimationComponent* animationComponent, uint32 eid, Animation* animation) {
	Component_Add(animationComponent, eid);
	animationComponent->animations[eid] = animation;
}