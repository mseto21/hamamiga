#include "InteractionTypes.h"
#include "ComponentBag.h"
#include "RectangleComponent.h"
#include "MovementComponent.h"
#include "TextureComponent.h"
#include "InputComponent.h"
#include "AnimationComponent.h"
#include "BulletComponent.h"
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
#include "EntityCache.h"

#include "Game.h"

// TO-DO: These includes weren't the best thing I've done.
#include <iostream>

// Some constants for hats
const int DamageReduction_ = 5;
const float JumpEnhancement_ = 1.41;
const float MaxYVelocityReduction_ = 0.5f;
const float MaxYVelocityEnchancement_ = 1.5f;


void Interaction_ApplyHatInteraction(int hatType, uint32 eid, uint32 hatEid, ComponentBag* cBag) {
	cBag->hatComponent->hats[eid].hat.hatType = hatType;
	cBag->hatComponent->hats[eid].hat.eid = hatEid;
	switch (hatType)  {
		case HatTypes_BunnyHat:
			Sound_Play(SoundCache_GetSound("hop"), 0);
			memcpy(&cBag->hatComponent->hats[eid].hat.name, "bunny", sizeof(cBag->hatComponent->hats[eid].hat.name));
			memcpy(&cBag->hatComponent->hats[eid].hat.effect, "2x Jump!", sizeof(cBag->hatComponent->hats[eid].hat.effect));
			cBag->movementComponent->movementValues[eid].accelY *= JumpEnhancement_;
			cBag->movementComponent->movementValues[eid].maxYVelocity *= JumpEnhancement_;
			cBag->hatComponent->hats[eid].hat.id = GlamourHatId_None;
			break;
		case HatTypes_HardHat:
			Sound_Play(SoundCache_GetSound("hatpickup"), 0);
			memcpy(&cBag->hatComponent->hats[eid].hat.name, "construction", sizeof(cBag->hatComponent->hats[eid].hat.name));
			memcpy(&cBag->hatComponent->hats[eid].hat.effect, "Take Half Damage!", sizeof(cBag->hatComponent->hats[eid].hat.effect));
			cBag->healthComponent->damageReduction[eid] = DamageReduction_;
			cBag->hatComponent->hats[eid].hat.id = GlamourHatId_None;
			break;
		case HatTypes_Cowboy:
			Sound_Play(SoundCache_GetSound("western"), 0);
			memcpy(&cBag->hatComponent->hats[eid].hat.name, "cowboy", sizeof(cBag->hatComponent->hats[eid].hat.name));
			memcpy(&cBag->hatComponent->hats[eid].hat.effect, "Press [SPACE] to shoot bulelts at enemies!", sizeof(cBag->hatComponent->hats[eid].hat.effect));
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
			Sound_Play(SoundCache_GetSound("hatpickup"), 0);
			memcpy(&cBag->hatComponent->hats[eid].gHat.name, "miner", sizeof(cBag->hatComponent->hats[eid].gHat.name));
			memcpy(&cBag->hatComponent->hats[eid].gHat.effect, "Let There be light!", sizeof(cBag->hatComponent->hats[eid].gHat.effect));
			cBag->hatComponent->hats[eid].gHat.id = GlamourHatId_Miner;
			break;
	    case HatTypes_Propeller:
	    	Sound_Play(SoundCache_GetSound("heli"), 0);
			memcpy(&cBag->hatComponent->hats[eid].hat.name, "propeller", sizeof(cBag->hatComponent->hats[eid].hat.name));
			memcpy(&cBag->hatComponent->hats[eid].hat.effect, "Fly with W and S!", sizeof(cBag->hatComponent->hats[eid].hat.effect));
			cBag->hatComponent->hats[eid].hat.id = GlamourHatId_None;
			cBag->movementComponent->movementValues[eid].flying = true;
			cBag->movementComponent->movementValues[eid].maxXVelocity *= MaxYVelocityEnchancement_;
			cBag->movementComponent->movementValues[eid].maxYVelocity *= MaxYVelocityReduction_;
			break;
	  case HatTypes_Beer:
	 		Sound_Play(SoundCache_GetSound("beer"), 0);
			memcpy(&cBag->hatComponent->hats[eid].gHat.name, "beer", sizeof(cBag->hatComponent->hats[eid].gHat.name));
			memcpy(&cBag->hatComponent->hats[eid].gHat.effect, "Tipsy at Work!", sizeof(cBag->hatComponent->hats[eid].gHat.effect));
			cBag->movementComponent->movementValues[eid].accelX *= -1;
			cBag->movementComponent->movementValues[eid].accelY *= -1;
			cBag->hatComponent->hats[eid].gHat.id = GlamourHatId_Beer;
			break;
		default:
			std::cerr << "Error: Unknown hat type given." << std::endl;
			break;
	}
}

void Interaction_RemoveHatInteraction(uint32 eid, ComponentBag* cBag) {
	if (!Component_HasIndex(cBag->hatComponent, eid)) {
		return;
	}
	Hat* hat = &cBag->hatComponent->hats[eid].hat;
	uint32 hatEid = hat->eid;
	switch (hat->hatType)  {
		case HatTypes_BunnyHat:
			cBag->movementComponent->movementValues[eid].accelY /= JumpEnhancement_;
			cBag->movementComponent->movementValues[eid].maxYVelocity /= JumpEnhancement_;
			break;
		case HatTypes_HardHat:
			cBag->healthComponent->damageReduction[eid] = 0;
			break;
	    case HatTypes_Propeller:
			cBag->movementComponent->movementValues[eid].flying = false;
			cBag->movementComponent->movementValues[eid].maxXVelocity /= MaxYVelocityEnchancement_;
			cBag->movementComponent->movementValues[eid].maxYVelocity /= MaxYVelocityReduction_;
			break;
	  	case HatTypes_Beer:
			cBag->movementComponent->movementValues[eid].accelX /= -1;
			cBag->movementComponent->movementValues[eid].accelY /= -1;
			break;
		case HatTypes_Cowboy:
			break;
		default:
			return;
	}
	Component_EnableEntity(cBag, hatEid);
	Rectangle dropperRect = cBag->rectangleComponent->entityRectangles[eid];
	cBag->rectangleComponent->entityRectangles[hatEid].x = (dropperRect.w / 2 - cBag->rectangleComponent->entityRectangles[hatEid].w / 2) + dropperRect.x;

	cBag->rectangleComponent->entityRectangles[hatEid].y = dropperRect.y - cBag->rectangleComponent->entityRectangles[hatEid].h;
	cBag->movementComponent->movementValues[hatEid].yVelocity = cBag->movementComponent->movementValues[eid].yVelocity - cBag->movementComponent->movementValues[hatEid].accelY;
	int dir = 1;
	if (cBag->movementComponent->movementValues[eid].left)
		dir = -1;
	cBag->movementComponent->movementValues[hatEid].xAccel = cBag->movementComponent->movementValues[eid].xVelocity + cBag->movementComponent->movementValues[hatEid].accelX * dir;

  	cBag->hatComponent->hats[eid].hat.id = -1;
  	cBag->hatComponent->hats[eid].hat.hatType = -1;
    cBag->hatComponent->hats[eid].hat.eid = -1;
    memcpy(hat->name, "", sizeof(String128));
    memcpy(hat->effect, "", sizeof(String128));
}


void Interaction_DisplayMessage(Game* game, Texture** txt) {
	if (!txt)
		return;
	Game_TriggerPause(game, txt);
}


void Interaction_PlayEventInteraction(uint32 eid, ComponentBag* cBag) {
	if (!Component_HasIndex(cBag->hatComponent, eid)) {
		return;
	}

	Hat* hat = &cBag->hatComponent->hats[eid].hat;
	switch (hat->hatType) {
		case HatTypes_Cowboy:
			{ // Scoped for C reasons...
				Rectangle rect = cBag->rectangleComponent->entityRectangles[eid];
				Entity* newBullet = EntityCache_GetNewEntity();
				BulletComponent_Add(cBag->bulletComponent, cBag->physicsComponent,
					cBag->aliveComponent, cBag->textureComponent, cBag->movementComponent,
					cBag->rectangleComponent, rect, newBullet->eid, true,
					cBag->movementComponent->movementValues[eid].left);
			}
			break;
		default:
			break;
	}
}
