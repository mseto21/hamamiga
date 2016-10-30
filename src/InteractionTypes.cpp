#include "InteractionTypes.h"
#include "ComponentBag.h"
#include "RectangleComponent.h"
#include "MovementComponent.h"
#include "TextureComponent.h"
#include "InputComponent.h"
#include "AnimationComponent.h"
#include "PhysicsComponent.h"
#include "HealthComponent.h"
#include "CameraComponent.h"
#include "HatComponent.h"
#include "AIComponent.h"
#include "AliveComponent.h"
#include "GoalComponent.h"
#include "InteractableComponent.h"
#include "SoundCache.h"
#include "GlamourHatEnum.h"
// TO-DO: These includes weren't the best thing I've done.
#include <iostream>


// Some constants for hats
const int DamageReduction_ = 5;
const float JumpEnchancement_ = 1.41;

enum HatTypes {
	HatTypes_BunnyHat = 0,
	HatTypes_HardHat,
	HatTypes_Crown,
	HatTypes_Disco,
	HatTypes_Miner,
};

void ApplyHatInteraction(int hatType, uint32 eid, ComponentBag* cBag) {
	Sound_Play(SoundCache_GetSound("hatpickup"), 0);
	switch (hatType)  {
		case HatTypes_BunnyHat:
			memcpy(&cBag->hatComponent->hats[eid].hat.name, "bunny", sizeof(cBag->hatComponent->hats[eid].hat.name));
			memcpy(&cBag->hatComponent->hats[eid].hat.effect, "2x jump", sizeof(cBag->hatComponent->hats[eid].hat.effect));
			cBag->movementComponent->movementValues[eid].accelY *= JumpEnchancement_;
			cBag->hatComponent->hats[eid].hat.id = GlamourHatId_None;
			break;
		case HatTypes_HardHat:
			memcpy(&cBag->hatComponent->hats[eid].hat.name, "construction", sizeof(cBag->hatComponent->hats[eid].hat.name));
			memcpy(&cBag->hatComponent->hats[eid].hat.effect, "1/2 Damage!", sizeof(cBag->hatComponent->hats[eid].hat.effect));
			cBag->healthComponent->damageReduction[eid] = DamageReduction_;
			cBag->hatComponent->hats[eid].hat.id = GlamourHatId_None;
			break;
		case HatTypes_Crown:
			cBag->goalComponent->winGoal[eid] = true;
			break;
		case HatTypes_Disco:
			memcpy(&cBag->hatComponent->hats[eid].gHat.name, "disco", sizeof(cBag->hatComponent->hats[eid].gHat.name));
			memcpy(&cBag->hatComponent->hats[eid].gHat.effect, "Strobe Lights!", sizeof(cBag->hatComponent->hats[eid].gHat.effect));
			cBag->hatComponent->hats[eid].gHat.id = GlamourHatId_Disco;
			Mix_VolumeMusic(MIX_MAX_VOLUME/4);
      		Sound_Play(SoundCache_GetSound("disco"), -1);
			break;
		case HatTypes_Miner:
			memcpy(&cBag->hatComponent->hats[eid].gHat.name, "miner", sizeof(cBag->hatComponent->hats[eid].gHat.name));
			memcpy(&cBag->hatComponent->hats[eid].gHat.effect, "Miner's hat!", sizeof(cBag->hatComponent->hats[eid].gHat.effect));
			cBag->hatComponent->hats[eid].hat.id = GlamourHatId_Miner;
			break;
		default:
			std::cerr << "Error: Unknown hat type given." << std::endl;
			break;
	}
}


void RemoveHatInteraction(int hatType, uint32 eid, ComponentBag* cBag) {

}