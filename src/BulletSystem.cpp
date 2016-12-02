#include "BulletSystem.h"
#include "Constants.h"
#include "Types.h"
#include "BulletComponent.h"
#include "RectangleComponent.h"
#include "PhysicsComponent.h"
#include "AliveComponent.h"
#include "TextureComponent.h"
#include "TextureCache.h"
#include "MovementComponent.h"
#include "ComponentBag.h"
#include "Zone.h"
#include "HatMethods.h"
#include "Interactions.h"
#include "EntityCache.h"

#include <SDL.h>
#include <iostream>

void BulletSystem_Initialize(BulletSystem* bulletSystem, ComponentBag* cBag, Zone* zone) {
  bulletSystem->physicsComponent    = cBag->physicsComponent;
  bulletSystem->rectangleComponent  = cBag->rectangleComponent;
  bulletSystem->bulletComponent     = cBag->bulletComponent;
  bulletSystem->aliveComponent      = cBag->aliveComponent;
  bulletSystem->textureComponent    = cBag->textureComponent;
  bulletSystem->movementComponent   = cBag->movementComponent;
  bulletSystem->cBag                = cBag;
  bulletSystem->zone                = zone;
}

void BulletSystem_Update(BulletSystem* bulletSystem, uint32 elapsed) {
	PhysicsComponent* physicsComponent  	 	= bulletSystem->physicsComponent;
	RectangleComponent* rectangleComponent          = bulletSystem->rectangleComponent;
	BulletComponent* bulletComponent  		= bulletSystem->bulletComponent;
	AliveComponent* aliveComponent  		= bulletSystem->aliveComponent;
	TextureComponent* textureComponent  		= bulletSystem->textureComponent;
	MovementComponent* movementComponent            = bulletSystem->movementComponent;

  for (uint32 entityIndex = 0; entityIndex < bulletComponent->count; entityIndex++) {
    uint32 eid = bulletComponent->entityArray[entityIndex];
    if (!Component_HasIndex(physicsComponent, eid) || !Component_HasIndex(rectangleComponent, eid)
    	|| !Component_HasIndex(bulletComponent, eid) || !Component_HasIndex(aliveComponent, eid)
    	|| !Component_HasIndex(textureComponent, eid) || !Component_HasIndex(movementComponent,eid)) {
      continue;
    } // make sure all components are there

    bulletComponent->bullet[eid].life += elapsed;

    //Getting player's information
    if (Component_HasIndex(rectangleComponent, Constants::PlayerIndex_)) {
    	Rectangle* playerRect = &rectangleComponent->entityRectangles[Constants::PlayerIndex_];
    	float maxScreenX = playerRect->x + Constants::ScreenWidth_;
    	float minScreenX = playerRect->x - Constants::ScreenWidth_;
    	float bX = rectangleComponent->entityRectangles[eid].x;
	MovementValues* moveValues = &movementComponent->movementValues[eid];
	if (!moveValues) {
	  std::cerr << "Error: No movement values for the input system to use." << std::endl;
	  continue;
	}
	moveValues->xAccel = 0;
	moveValues->yAccel = 0;
	if (bulletComponent->bullet[eid].left == true){
	  moveValues->xAccel = -moveValues->accelX;
	} else {
	  moveValues->xAccel = moveValues->accelX;
	}
	
	if (bX > bulletSystem->zone->levelWidth || bX < 0 || bX < minScreenX || bX > maxScreenX 
	    || bulletComponent->bullet[eid].collided == true) {
	  EntityCache_Remove(eid);
	  ComponentBag_ForceRemove(bulletSystem->cBag, eid);
	  
	  switch(bulletComponent->type[eid]) {
          case HatTypes_Chef:
            ChefHatSubtract();
            break;
          case HatTypes_Cowboy:
            CowboyHatSubtract();
            break;
          default:
            break;
	  }
	}
    }
  }
}

void BulletSystem_Free(BulletSystem* bulletSystem) {
  bulletSystem->physicsComponent = nullptr;
  bulletSystem->rectangleComponent = nullptr;
  bulletSystem->bulletComponent = nullptr;
  bulletSystem->aliveComponent = nullptr;
  bulletSystem->textureComponent = nullptr;
  bulletSystem->cBag = nullptr;
}

