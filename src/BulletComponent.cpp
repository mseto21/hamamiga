#include "BulletComponent.h"
#include "EntityCache.h"
#include "Interactions.h"
#include "ComponentBag.h"
#include "MovementComponent.h"
#include "AnimationComponent.h"
#include "TeamComponent.h"
#include "AIComponent.h"
#include "PhysicsComponent.h"
#include "DamageComponent.h"
#include "InteractableComponent.h"
#include "AliveComponent.h"
#include "RectangleComponent.h"
#include "TextureComponent.h"
#include "TextureCache.h"
#include "SoundCache.h"

#include <iostream>
#include <SDL.h>

void BulletComponent_Add(BulletComponent* bulletComponent, uint32 eid) {
	if (Component_HadIndex(bulletComponent, eid)) {
		Component_Enable(bulletComponent, eid);
		return;
	}
	Component_Add(bulletComponent, eid);

	for (int i = 0; i < MaxBullets_; i++) {
		bulletComponent->bulletValues[eid].bulletEids[i] = EntityCache_GetNewEntity()->eid;
		bulletComponent->bulletValues[eid].available[i] = true;
	}

	bulletComponent->bulletValues[eid].availableBullets = MaxBullets_;
}


void BulletComponent_Create(BulletComponent* bulletComponent, uint32 eid, ComponentBag* cBag, int type) {
	BulletValues* bulletValues = &bulletComponent->bulletValues[eid];
	if (bulletValues->availableBullets > 0) {
  		bulletValues->availableBullets--;
  	} else {
  		return;
  	}

  	int i;
  	for (i = 0; i < MaxBullets_; i++) {
		if (bulletComponent->bulletValues[eid].available[i]) {
			bulletComponent->bulletValues[eid].available[i] = false;
			break;
		}
	}

	uint32 bulletEid = bulletValues->bulletEids[i];
	Rectangle rect = cBag->rectangleComponent->entityRectangles[eid];
	bool left = cBag->movementComponent->movementValues[eid].left;

	Texture* texture;
	Animation animation;
	int t;
	int xVelocity = 7;
	int yVelocity = 0;
	int damage = 10;

	switch (type) {
		case HatTypes_Chef:
			xVelocity = 14;
			yVelocity = 9;
			texture = TextureCache_GetTexture("knife");
			Animation_Initialize(&animation, 1, 150.0, 34, 12);
			cBag->movementComponent->movementValues[bulletEid].yVelocity = yVelocity * -1 + cBag->movementComponent->movementValues[eid].yVelocity;
			t = InteractionTypes_Chef;
			damage = 35;
			Sound_Play(SoundCache_GetSound("knife"), 0);
			break;
		case HatTypes_Cowboy:
			texture = TextureCache_GetTexture("bullet");
			Animation_Initialize(&animation, 1, 150.0, 52, 12);
			t = InteractionTypes_Cowboy;
			damage = 35;
			Sound_Play(SoundCache_GetSound("pew"), 0);
			break;
	}

	MovementComponent_Add(cBag->movementComponent, bulletEid, xVelocity, yVelocity, xVelocity, yVelocity);
	TeamComponent_Add(cBag->teamComponent, bulletEid, 0);
	DamageComponent_Add(cBag->damageComponent, bulletEid, damage);
	AIComponent_Add(cBag->aiComponent, bulletEid, 1);
	PhysicsComponent_Add(cBag->physicsComponent, bulletEid, 1);
	InteractableComponent_Add(cBag->interactableComponent, bulletEid, 0, t, 0, 0);
	AnimationComponent_Add(cBag->animationComponent, bulletEid, animation);
	TextureComponent_Add(cBag->textureComponent, bulletEid, texture);
	RectangleComponent_Add(cBag->rectangleComponent, bulletEid, 0, 0, texture->w, texture->h);
	AliveComponent_Add(cBag->aliveComponent, bulletEid);
	
	cBag->interactableComponent->interacted[bulletEid] = false;
	cBag->movementComponent->movementValues[bulletEid].xVelocity = xVelocity;

	if (!left) {
		cBag->rectangleComponent->entityRectangles[bulletEid].x = rect.x + 50;
		cBag->rectangleComponent->entityRectangles[bulletEid].y = rect.y + 40;
	} else {
		texture->flip = SDL_FLIP_HORIZONTAL;
		cBag->rectangleComponent->entityRectangles[bulletEid].x = rect.x - texture->w;
		cBag->rectangleComponent->entityRectangles[bulletEid].y = rect.y + 40;
		cBag->movementComponent->movementValues[bulletEid].xVelocity *= -1;
	}
}





