#include "Interactions.h"
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
#include "DamageComponent.h"
#include "InteractableComponent.h"
#include "TeamComponent.h"
#include "BulletComponent.h"
#include "SoundCache.h"
#include "GlamourHatEnum.h"
#include "EntityCache.h"
#include "StatSystem.h"
#include "HatMethods.h"
#include "StateLoader.h"
#include "TextureCache.h"


#include "Game.h"
#include <iostream>

// Some constants for hats
const int DamageReduction_ = 5;
const float JumpEnhancement_ = 1.41;
const float MaxYVelocityReduction_ = 0.5f;
const float MaxYVelocityEnchancement_ = 1.5f;

//constants for hat score file
const char * hatpath = "assets/score/hats.txt";
const char * val = "1";

void AddHatScore(int type){
	if (hatsCollected[type] == 0){
		hatsCollected[type] = 1;
		scores[Hats_] += 1;
	}
}

bool Interaction_ApplyHatInteraction(int hatType, uint32 eid, uint32 hatEid, ComponentBag* cBag) {
	if (!Component_HasIndex(cBag->hatComponent, eid)){
		return false;
	}
	switch (hatType)  {
		case HatTypes_BunnyHat:
			if (cBag->hatComponent->hats[eid].hat.hatType != HatTypes_Empty)
				return false;
			cBag->hatComponent->hats[eid].hat.hatType = hatType;
			cBag->hatComponent->hats[eid].hat.eid = hatEid;
			Sound_Play(SoundCache_GetSound("hop"), 0);
			memcpy(&cBag->hatComponent->hats[eid].hat.name, "bunny", sizeof(cBag->hatComponent->hats[eid].hat.name));
			memcpy(&cBag->hatComponent->hats[eid].hat.effect, "2x Jump!", sizeof(cBag->hatComponent->hats[eid].hat.effect));
			cBag->movementComponent->movementValues[eid].accelY *= JumpEnhancement_;
			cBag->movementComponent->movementValues[eid].maxYVelocity *= JumpEnhancement_;
			cBag->hatComponent->hats[eid].hat.id = GlamourHatId_None;
			Scores_Update(hatpath, (char*)"bunny", val);
			AddHatScore(HatTypes_BunnyHat);
			break;
		case HatTypes_HardHat:
			if (cBag->hatComponent->hats[eid].hat.hatType != HatTypes_Empty)
				return false;
			cBag->hatComponent->hats[eid].hat.hatType = hatType;
			cBag->hatComponent->hats[eid].hat.eid = hatEid;
			Sound_Play(SoundCache_GetSound("hatpickup"), 0);
			memcpy(&cBag->hatComponent->hats[eid].hat.name, "construction", sizeof(cBag->hatComponent->hats[eid].hat.name));
			memcpy(&cBag->hatComponent->hats[eid].hat.effect, "Take Half Damage!", sizeof(cBag->hatComponent->hats[eid].hat.effect));
			cBag->healthComponent->damageReduction[eid] = DamageReduction_;
			cBag->hatComponent->hats[eid].hat.id = GlamourHatId_None;
			Scores_Update(hatpath, (char*)"hard", val);
			AddHatScore(HatTypes_HardHat);
			break;
		case HatTypes_Cowboy:
			if (cBag->hatComponent->hats[eid].hat.hatType != HatTypes_Empty)
				return false;
			cBag->hatComponent->hats[eid].hat.hatType = hatType;
			cBag->hatComponent->hats[eid].hat.eid = hatEid;
			Sound_Play(SoundCache_GetSound("western"), 0);
			memcpy(&cBag->hatComponent->hats[eid].hat.name, "cowboy", sizeof(cBag->hatComponent->hats[eid].hat.name));
			memcpy(&cBag->hatComponent->hats[eid].hat.effect, "Press [SPACE] to shoot bulelts at enemies!", sizeof(cBag->hatComponent->hats[eid].hat.effect));
			cBag->hatComponent->hats[eid].hat.id = GlamourHatId_None;
			Scores_Update(hatpath, (char*)"cowboy", val);
			AddHatScore(HatTypes_Cowboy);
			BulletComponent_Add(cBag->bulletComponent, eid);
			break;
		case HatTypes_Crown:
			cBag->goalComponent->winGoal[eid] = true;
			Scores_Update(hatpath, (char*)"crown", val);
			AddHatScore(HatTypes_Crown);
			break;
		case HatTypes_Disco:
			memcpy(&cBag->hatComponent->hats[eid].gHat.name, "disco", sizeof(cBag->hatComponent->hats[eid].gHat.name));
			memcpy(&cBag->hatComponent->hats[eid].gHat.effect, "Strobe Lights!", sizeof(cBag->hatComponent->hats[eid].gHat.effect));
			cBag->hatComponent->hats[eid].gHat.hatType = hatType;
			cBag->hatComponent->hats[eid].gHat.eid = hatEid;
			cBag->hatComponent->hats[eid].gHat.id = GlamourHatId_Disco;
			Mix_VolumeMusic(MIX_MAX_VOLUME/4);
			Sound_Play(SoundCache_GetSound("disco"), -1);
			Scores_Update(hatpath, (char*)"disco", val);
			AddHatScore(HatTypes_Disco);
			break;
		case HatTypes_Miner:
			Sound_Play(SoundCache_GetSound("hatpickup"), 0);
			memcpy(&cBag->hatComponent->hats[eid].gHat.name, "miner", sizeof(cBag->hatComponent->hats[eid].gHat.name));
			memcpy(&cBag->hatComponent->hats[eid].gHat.effect, "Let There be light!", sizeof(cBag->hatComponent->hats[eid].gHat.effect));
			cBag->hatComponent->hats[eid].gHat.hatType = hatType;
			cBag->hatComponent->hats[eid].gHat.eid = hatEid;
			cBag->hatComponent->hats[eid].gHat.id = GlamourHatId_Miner;
			Scores_Update(hatpath, (char*)"miner", val);
			AddHatScore(HatTypes_Miner);
			break;
	    case HatTypes_Propeller:
	    	if (cBag->hatComponent->hats[eid].hat.hatType != HatTypes_Empty)
				return false;
			cBag->hatComponent->hats[eid].hat.hatType = hatType;
			cBag->hatComponent->hats[eid].hat.eid = hatEid;
	    Sound_Play(SoundCache_GetSound("heli"), 0);
			memcpy(&cBag->hatComponent->hats[eid].hat.name, "propeller", sizeof(cBag->hatComponent->hats[eid].hat.name));
			memcpy(&cBag->hatComponent->hats[eid].hat.effect, "Fly with W and S!", sizeof(cBag->hatComponent->hats[eid].hat.effect));
			cBag->movementComponent->movementValues[eid].flying = true;
			cBag->movementComponent->movementValues[eid].maxXVelocity *= MaxYVelocityEnchancement_;
			cBag->movementComponent->movementValues[eid].maxYVelocity *= MaxYVelocityReduction_;
			cBag->hatComponent->hats[eid].hat.id = GlamourHatId_None;
			Scores_Update(hatpath, (char*)"prop", val);
			AddHatScore(HatTypes_Propeller);
			break;
	  case HatTypes_Beer:
	 		Sound_Play(SoundCache_GetSound("beer"), 0);
			memcpy(&cBag->hatComponent->hats[eid].gHat.name, "beer", sizeof(cBag->hatComponent->hats[eid].gHat.name));
			memcpy(&cBag->hatComponent->hats[eid].gHat.effect, "Tipsy at Work!", sizeof(cBag->hatComponent->hats[eid].gHat.effect));
			cBag->movementComponent->movementValues[eid].accelX *= -1;
			cBag->movementComponent->movementValues[eid].accelY *= -1;
			cBag->movementComponent->movementValues[eid].drunk = true;
			cBag->hatComponent->hats[eid].gHat.hatType = hatType;
			cBag->hatComponent->hats[eid].gHat.eid = hatEid;
			cBag->hatComponent->hats[eid].gHat.id = GlamourHatId_Beer;
			Scores_Update(hatpath, (char*)"beer", val);
			AddHatScore(HatTypes_Beer);
			break;
	  case HatTypes_Chef:
	        if (cBag->hatComponent->hats[eid].hat.hatType != HatTypes_Empty)
			      return false;
			cBag->hatComponent->hats[eid].hat.hatType = hatType;
			cBag->hatComponent->hats[eid].hat.eid = hatEid;
			Sound_Play(SoundCache_GetSound("chef"), 0);
			memcpy(&cBag->hatComponent->hats[eid].hat.name, "chef", sizeof(cBag->hatComponent->hats[eid].hat.name));
			memcpy(&cBag->hatComponent->hats[eid].hat.effect, "Press [SPACE] to throw your knives at enemies!", sizeof(cBag->hatComponent->hats[eid].hat.effect));
			cBag->hatComponent->hats[eid].hat.id = GlamourHatId_None;
			BulletComponent_Add(cBag->bulletComponent, eid);
			Scores_Update(hatpath, (char*)"chef", val);
			AddHatScore(HatTypes_Chef);
			break;
		case HatTypes_Circus:
			cBag->hatComponent->hats[eid].gHat.hatType = hatType;
			cBag->hatComponent->hats[eid].gHat.eid = hatEid;
			Sound_Play(SoundCache_GetSound("circus"), 0);
			memcpy(&cBag->hatComponent->hats[eid].gHat.name, "circus", sizeof(cBag->hatComponent->hats[eid].gHat.name));
			memcpy(&cBag->hatComponent->hats[eid].gHat.effect, "Press [SPACE] to spin", sizeof(cBag->hatComponent->hats[eid].gHat.effect));
			cBag->hatComponent->hats[eid].gHat.id = GlamourHatId_Circus;
			Scores_Update(hatpath, (char*)"circus", val);
			AddHatScore(HatTypes_Circus);
			break;
		default:
			std::cerr << "Error: Unknown hat type given." << std::endl;
			return false;
	}
	return true;
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
			cBag->movementComponent->movementValues[eid].drunk = false;
			break;
		case HatTypes_Chef:
		case HatTypes_Cowboy:
			Component_Disable(cBag->bulletComponent, eid);
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

  	cBag->hatComponent->hats[eid].hat.id = GlamourHatId_None;
  	cBag->hatComponent->hats[eid].hat.hatType = HatTypes_Empty;
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
		case HatTypes_Cowboy: {
	      	BulletComponent_Create(cBag->bulletComponent, eid, cBag, HatTypes_Cowboy);
			break;
		}
		case HatTypes_Chef: {
		    BulletComponent_Create(cBag->bulletComponent, eid, cBag, HatTypes_Chef);
			break;
		}
		default:
			break;
	}
}


void Interaction_EnterDoor(Game* game, uint32 level) {
	game->playState.levelSelection = level;
	game->gameState = GameState_ReturnAndEnterLevel;
}


