#include "InteractionSystem.h"
#include "Interactions.h"
#include "RectangleComponent.h"
#include "InputComponent.h"
#include "InteractableComponent.h"
#include "BulletComponent.h"
#include "AliveComponent.h"
#include "PhysicsComponent.h"
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
		if (type == InteractionTypes_Chef || type == InteractionTypes_Cowboy) {
			if (physicsComponent->physicsValues[eid].collided) {
				bulletComponent->bulletValues[Constants::PlayerIndex_].availableBullets++;
				interactableComponent->interacted[eid] = true;
				if (Component_HasIndex(aliveComponent, eid)) {
			  		aliveComponent->alive[eid] = false;
		  		}
		  		physicsComponent->physicsValues[eid].collided = false;
		  		inputComponent->interact[Constants::PlayerIndex_] = false;
			}
			continue;
		}

		if (!Collision(r1, rectangleComponent->entityRectangles[eid])) {
			interactableComponent->canBeInteractedWith[eid] = false;
			continue;
		}

		interactableComponent->canBeInteractedWith[eid] = true;
		bool interact = inputComponent->interact[Constants::PlayerIndex_];

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
					  	inputComponent->interact[Constants::PlayerIndex_] = false;
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
				if (interact) {
					inputComponent->interact[Constants::PlayerIndex_] = false;
					Interaction_EnterDoor(interactionSystem->game, interactableComponent->datafield[eid]);
				}
				continue;
			case InteractionTypes_Chef:
			case InteractionTypes_Cowboy:
				if (physicsComponent->physicsValues[eid].collided) {
					bulletComponent->bulletValues[Constants::PlayerIndex_].availableBullets++;
					interactableComponent->interacted[eid] = true;
					if (Component_HasIndex(aliveComponent, eid)) {
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
	interactionSystem->physicsComponent = nullptr;
	interactionSystem->game = nullptr;
}