#include "InteractionSystem.h"
#include "Interactions.h"
#include "RectangleComponent.h"
#include "InputComponent.h"
#include "InteractableComponent.h"
#include "BulletComponent.h"
#include "AliveComponent.h"
#include "GoalComponent.h"
#include "ComponentBag.h"
#include "SoundCache.h"
#include "HatMethods.h"

#include <iostream>

void InteractionSystem_Initialize(InteractionSystem* interactionSystem, ComponentBag* cBag, Game* game) {
	interactionSystem->interactableComponent 	= cBag->interactableComponent;
	interactionSystem->inputComponent 			= cBag->inputComponent;
	interactionSystem->rectangleComponent 		= cBag->rectangleComponent;
	interactionSystem->aliveComponent 			= cBag->aliveComponent;
	interactionSystem->goalComponent 			= cBag->goalComponent;
	interactionSystem->bulletComponent 			= cBag->bulletComponent;
	interactionSystem->cBag 					= cBag;
	interactionSystem->game 					= game;
}

static bool Collision(const Rectangle r1, const Rectangle r2) {
  return r1.x <= r2.x + r2.w 
  		&& r1.x + r1.w >= r2.x 
  		&& r1.y <= r2.y + r2.h 
  		&& r1.y + r1.h >= r2.y;
}

void InteractionSystem_Update(InteractionSystem* interactionSystem) {
	InteractableComponent* interactableComponent = interactionSystem->interactableComponent;
	InputComponent* inputComponent = interactionSystem->inputComponent;
	RectangleComponent* rectangleComponent = interactionSystem->rectangleComponent;
	AliveComponent* aliveComponent = interactionSystem->aliveComponent;
	GoalComponent* goalComponent = interactionSystem->goalComponent;
	BulletComponent* bulletComponent = interactionSystem->bulletComponent;

	Rectangle r1 = rectangleComponent->entityRectangles[Constants::PlayerIndex_];

	for (uint32 entityIndex = 0; entityIndex < interactableComponent->count; entityIndex++) {
		uint32 eid = interactableComponent->entityArray[entityIndex];
		if (!Collision(r1, rectangleComponent->entityRectangles[eid])) {
			interactableComponent->canBeInteractedWith[eid] = false;
			continue;
		}

		interactableComponent->canBeInteractedWith[eid] = true;
		bool interact = inputComponent->interact[Constants::PlayerIndex_];

		int type = interactableComponent->types[eid];
		switch (type) {
			case InteractionTypes_Hat: {
				if (interact) {
					int hattype = interactableComponent->datafield[eid];
					if (!interactableComponent->interacted[eid]) {
						if (Interaction_ApplyHatInteraction(hattype, Constants::PlayerIndex_, eid, interactionSystem->cBag)) {
							Interaction_DisplayMessage(interactionSystem->game, interactableComponent->txt[eid]);
							interactableComponent->interacted[eid] = true;
							if (Component_HasIndex(aliveComponent, eid)) {
						  		aliveComponent->alive[eid] = false;
					  		}
					  	}
					}
				}
				continue;
			}
			case InteractionTypes_Coin:
				goalComponent->points[Constants::PlayerIndex_] += Constants::CoinValue_;
				if (Component_HasIndex(aliveComponent, eid)) {
					Sound_Play(SoundCache_GetSound("coin"), 0);
					aliveComponent->alive[eid] = false;
				}
				continue;
			case InteractionTypes_Door:
				if (interact)
					Interaction_EnterDoor(interactionSystem->game, interactableComponent->datafield[eid]);
				continue;
			case InteractionTypes_Chef:
			case InteractionTypes_Cowboy:
				if (interact) {
					if (!interactableComponent->interacted[eid]) {
						if (bulletComponent->bulletValues[Constants::PlayerIndex_].availableBullets < 3)
							bulletComponent->bulletValues[Constants::PlayerIndex_].availableBullets++;
						interactableComponent->interacted[eid] = true;
						aliveComponent->alive[eid] = false;
					}
				}
				continue;
			default:
				continue;
		}
	}
}

void InteractionSystem_Free(InteractionSystem* interactionSystem) {
	interactionSystem->interactableComponent = nullptr;
	interactionSystem->inputComponent = nullptr;
	interactionSystem->rectangleComponent = nullptr;
	interactionSystem->game = nullptr;
}