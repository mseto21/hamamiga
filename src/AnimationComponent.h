#include "Animation.h"
#include "Component.h"

struct AnimationComponent : public Component {
	Animation animations[Constants::MaxEntities_];
};

void AnimationComponent_Add(AnimationComponent* animationComponent, uint32 eid, Animation animation);
