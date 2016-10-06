#include "Component.h"

struct Animation;

struct AnimationComponent : public Component {
	Animation* animations[Constants::MaxEntities_];
};

void AnimationComponent_Add(AnimationComponent* animationComponent, uint32 eid, Animation* animation);
