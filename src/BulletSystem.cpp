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

#include <SDL.h>
#include <iostream>

void BulletSystem_Initialize(BulletSystem* bulletSystem, ComponentBag* cBag) {
  bulletSystem->physicsComponent    = cBag->physicsComponent;
  bulletSystem->rectangleComponent  = cBag->rectangleComponent;
  bulletSystem->bulletComponent     = cBag->bulletComponent;
  bulletSystem->aliveComponent  		= cBag->aliveComponent;
  bulletSystem->textureComponent  	= cBag->textureComponent;
  bulletSystem->movementComponent   = cBag->movementComponent;
}

void BulletSystem_Update(BulletSystem* bulletSystem) {
	PhysicsComponent* physicsComponent  	 	= bulletSystem->physicsComponent;
  RectangleComponent* rectangleComponent  = bulletSystem->rectangleComponent;
  BulletComponent* bulletComponent  			= bulletSystem->bulletComponent;
  AliveComponent* aliveComponent  				= bulletSystem->aliveComponent;
  TextureComponent* textureComponent  		= bulletSystem->textureComponent;
  MovementComponent* movementComponent    = bulletSystem->movementComponent;

  uint16 bulletsRemoved = 0;
  for (uint32 entityIndex = 0; entityIndex < bulletComponent->count; entityIndex++) {
    uint32 eid = bulletComponent->entityArray[entityIndex];

    if (!Component_HasIndex(physicsComponent, eid) || !Component_HasIndex(rectangleComponent, eid)
    	|| !Component_HasIndex(bulletComponent, eid) || !Component_HasIndex(aliveComponent, eid)
    	|| !Component_HasIndex(textureComponent, eid) || !Component_HasIndex(movementComponent,eid)) {
      continue;
    } // make sure all components are there

    //Getting player's information
    if (Component_HasIndex(rectangleComponent, Constants::PlayerIndex_)){
    	Rectangle* playerRect = &rectangleComponent->entityRectangles[Constants::PlayerIndex_];
    	float maxScreenX = playerRect->x + (Constants::ScreenWidth_ /2);
    	float minScreenX = playerRect->x - (Constants::ScreenWidth_ /2);
    	float bX = rectangleComponent->entityRectangles[eid].x;
    MovementValues* moveValues = &movementComponent->movementValues[eid];
    if (!moveValues) {
      std::cerr << "Error: No movement values for the input system to use." << std::endl;
      continue;
    }
    moveValues->xAccel = 0;
    moveValues->yAccel = 0;
    moveValues->xAccel = moveValues->accelX;
    
	    if (bX > Constants::LevelWidth_ || bX < 0 || bX < minScreenX || bX > maxScreenX ||
        bulletComponent->bullet[eid].collided == true){
	    	aliveComponent->alive[eid] = false;
	    	bulletsRemoved++;
	    }
		}
  }
  //update bullet count
  bulletComponent->count -= bulletsRemoved;
}

void BulletSystem_Free(BulletSystem* bulletSystem) {
  bulletSystem->physicsComponent = nullptr;
  bulletSystem->rectangleComponent = nullptr;
  bulletSystem->bulletComponent = nullptr;
  bulletSystem->aliveComponent = nullptr;
  bulletSystem->textureComponent = nullptr;
}

