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

#include <iostream>

void BulletComponent_Add(BulletComponent* bulletComponent, uint32 eid) {
	Component_Add(bulletComponent, eid);
	for (int i = 0; i < MaxBullets_; i++) {
		bulletComponent->bulletValues[eid].bulletEids[i] = EntityCache_GetNewEntity()->eid;
		bulletComponent->bulletValues[eid].initialized[i] = false;
	}

	bulletComponent->bulletValues[eid].availableBullets = 3;
}


void BulletComponent_Create(BulletComponent* bulletComponent, uint32 eid, ComponentBag* cBag, int type) {
	int index = bulletComponent->bulletValues[eid].availableBullets;
	uint32 bulletEid = bulletComponent->bulletValues[eid].bulletEids[bulletComponent->bulletValues[eid].availableBullets];
	Rectangle rect = cBag->rectangleComponent->entityRectangles[eid];
	bool left = cBag->movementComponent->movementValues[eid].left;

	Texture* texture;
	Animation animation;
	int t;
	switch (type) {
		case HatTypes_Chef:
			texture = TextureCache_GetTexture("knife");
			Animation_Initialize(&animation, 1, 150.0, 34, 12);
			cBag->movementComponent->movementValues[bulletEid].yVelocity = -7 + cBag->movementComponent->movementValues[eid].yVelocity;
			cBag->movementComponent->movementValues[bulletEid].xVelocity += cBag->movementComponent->movementValues[eid].xVelocity;
			t = InteractionTypes_Chef;
			break;
		case HatTypes_Cowboy:
			texture = TextureCache_GetTexture("bullet");
			Animation_Initialize(&animation, 1, 150.0, 52, 12);
			cBag->movementComponent->movementValues[bulletEid].xVelocity = 7;
			t = InteractionTypes_Cowboy;
			break;
	}

	if (!bulletComponent->bulletValues[eid].initialized[index]) {
		MovementComponent_Add(cBag->movementComponent, bulletEid, 7, 0, 1.5, 1.0);
		TeamComponent_Add(cBag->teamComponent, bulletEid, 0);
		DamageComponent_Add(cBag->damageComponent, bulletEid, 100);
		AIComponent_Add(cBag->aiComponent, bulletEid, 1);
		PhysicsComponent_Add(cBag->physicsComponent, bulletEid, 1);
		InteractableComponent_Add(cBag->interactableComponent, bulletEid, TextureCache_GetTexture("bullet-pickup"), t, 0, 0);
		AliveComponent_Add(cBag->aliveComponent, bulletEid);
		AnimationComponent_Add(cBag->animationComponent, bulletEid, animation);
		TextureComponent_Add(cBag->textureComponent, bulletEid, texture);
		RectangleComponent_Add(cBag->rectangleComponent, bulletEid, 0, 0, texture->w, texture->h);
		bulletComponent->bulletValues[eid].initialized[index] = true;
	}

	Component_EnableEntity(cBag, bulletEid);
	cBag->interactableComponent->interacted[bulletEid] = false;
	cBag->aliveComponent->alive[bulletEid] = true;

	if (!left) {
		cBag->rectangleComponent->entityRectangles[bulletEid].x = rect.x + 50;
		cBag->rectangleComponent->entityRectangles[bulletEid].y = rect.y + 40;
	} else {
		cBag->rectangleComponent->entityRectangles[bulletEid].x = rect.x - texture->w;
		cBag->rectangleComponent->entityRectangles[bulletEid].y = rect.y + 40;
	}
}





