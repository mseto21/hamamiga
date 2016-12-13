#include "InteractionSystem.h"
#include "Interactions.h"
#include "RectangleComponent.h"
#include "InputComponent.h"
#include "InteractableComponent.h"
#include "BulletComponent.h"
#include "AliveComponent.h"
#include "PhysicsComponent.h"
#include "GoalComponent.h"
#include "DamageComponent.h"
#include "ComponentBag.h"
#include "SoundCache.h"
#include "HatMethods.h"
#include "StatSystem.h"

#include <iostream>

void InteractionSystem_Initialize(InteractionSystem* interactionSystem, ComponentBag* cBag, Game* game) {
	interactionSystem->interactableComponent 	= cBag->interactableComponent;
	interactionSystem->inputComponent 			= cBag->inputComponent;
	interactionSystem->rectangleComponent 		= cBag->rectangleComponent;
	interactionSystem->aliveComponent 			= cBag->aliveComponent;
	interactionSystem->goalComponent 			= cBag->goalComponent;
	interactionSystem->bulletComponent 			= cBag->bulletComponent;
	interactionSystem->physicsComponent 		= cBag->physicsComponent;
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
	PhysicsComponent* physicsComponent = interactionSystem->physicsComponent;

	Rectangle r1 = rectangleComponent->entityRectangles[Constants::PlayerIndex_];

	for (uint32 entityIndex = 0; entityIndex < interactableComponent->count; entityIndex++) {
		uint32 eid = interactableComponent->entityArray[entityIndex];

		int type = interactableComponent->types[eid];
		interactableComponent->canBeInteractedWith[eid] = true;
		bool interact = inputComponent->interact[Constants::PlayerIndex_];

		switch (type) {
			case InteractionTypes_Hat: {
				if (!Collision(r1, rectangleComponent->entityRectangles[eid])) {
					interactableComponent->canBeInteractedWith[eid] = false;
					continue;
				}
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
					  	inputComponent->interact[Constants::PlayerIndex_] = false;
					}
				}
				continue;
			}
			case InteractionTypes_Coin:
				if (!Collision(r1, rectangleComponent->entityRectangles[eid])) {
					interactableComponent->canBeInteractedWith[eid] = false;
					continue;
				}
				goalComponent->points[Constants::PlayerIndex_] += Constants::CoinValue_;
				if (Component_HasIndex(aliveComponent, eid)) {
					Sound_Play(SoundCache_GetSound("coin"), 0);
					scores[Coins_] += 1;
					aliveComponent->alive[eid] = false;
				}
				continue;
			case InteractionTypes_Door:
				if (!Collision(r1, rectangleComponent->entityRectangles[eid])) {
					interactableComponent->canBeInteractedWith[eid] = false;
					continue;
				}
				if (interact) {
					inputComponent->interact[Constants::PlayerIndex_] = false;
					Interaction_EnterDoor(interactionSystem->game, interactableComponent->datafield[eid]);
				}
				continue;
			case InteractionTypes_Chef:
				if (physicsComponent->physicsValues[eid].collided) {
					Component_Disable(interactionSystem->cBag->damageComponent, eid);
				}
				if (Collision(r1, rectangleComponent->entityRectangles[eid])) {
					physicsComponent->physicsValues[eid].collided = false;
					for (int i = 0; i < MaxBullets_; i++) {
						if (bulletComponent->bulletValues[Constants::PlayerIndex_].bulletEids[i] == eid)
							bulletComponent->bulletValues[Constants::PlayerIndex_].available[i] = true;
					}
					bulletComponent->bulletValues[Constants::PlayerIndex_].availableBullets++;
					if (bulletComponent->bulletValues[Constants::PlayerIndex_].availableBullets > MaxBullets_) {
						std::cerr << "InteractionSystem: Added too many bullets!" << std::endl;
						bulletComponent->bulletValues[Constants::PlayerIndex_].availableBullets = MaxBullets_;
					}
					ComponentBag_ForceRemove(interactionSystem->cBag , eid);
				}
				continue;
			case InteractionTypes_Cowboy:
				// Get rid of a regular bullet upon any colllision.
				if (Collision(r1, rectangleComponent->entityRectangles[eid]) || physicsComponent->physicsValues[eid].collided || aliveComponent->timeAlive[eid] > MaxBulletLife_) {
					for (int i = 0; i < MaxBullets_; i++) {
						if (bulletComponent->bulletValues[Constants::PlayerIndex_].bulletEids[i] == eid)
							bulletComponent->bulletValues[Constants::PlayerIndex_].available[i] = true;
					}
					bulletComponent->bulletValues[Constants::PlayerIndex_].availableBullets++;
					physicsComponent->physicsValues[eid].collided = false;
					ComponentBag_ForceRemove(interactionSystem->cBag, eid);
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
	interactionSystem->physicsComponent = nullptr;
	interactionSystem->game = nullptr;
}