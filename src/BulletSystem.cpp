#include "BulletSystem.h"
#include "Constants.h"
#include "Types.h"
#include "BulletComponent.h"
#include "RectangleComponent.h"
#include "PhysicsComponent.h"
#include "AliveComponent.h"
#include "TextureComponent.h"
#include "TextureCache.h"
#include "ComponentBag.h"

#include <SDL.h>
#include <iostream>

void BulletSystem_Initialize(BulletSystem* bulletSystem, ComponentBag* cBag) {
  bulletSystem->physicsComponent    = cBag->physicsComponent;
  bulletSystem->rectangleComponent  = cBag->rectangleComponent;
  bulletSystem->bulletComponent     = cBag->bulletComponent;
  bulletSystem->aliveComponent  		= cBag->aliveComponent;
  bulletSystem->textureComponent  	= cBag->textureComponent;
}

void BulletSystem_Update(BulletSystem* bulletSystem) {
	PhysicsComponent* physicsComponent  	 	= bulletSystem->physicsComponent;
  RectangleComponent* rectangleComponent  = bulletSystem->rectangleComponent;
  BulletComponent* bulletComponent  			= bulletSystem->bulletComponent;
  AliveComponent* aliveComponent  				= bulletSystem->aliveComponent;
  TextureComponent* textureComponent  		= bulletSystem->textureComponent;

  for (uint32 entityIndex = 0; entityIndex < bulletComponent->count; entityIndex++) {
    uint32 eid = bulletComponent->entityArray[entityIndex];

    if (!Component_HasIndex(physicsComponent, eid) || !Component_HasIndex(rectangleComponent, eid)
    	|| !Component_HasIndex(bulletComponent, eid) || !Component_HasIndex(aliveComponent, eid)
    	|| !Component_HasIndex(textureComponent, eid)) {
      continue;
    } // make sure all components are there

    //Getting player's information
    if (Component_HasIndex(rectangleComponent, Constants::PlayerIndex_)){
    	Rectangle* playerRect = &rectangleComponent->entityRectangles[Constants::PlayerIndex_];
    	float maxScreenX = playerRect->x + (Constants::ScreenWidth_ /2);
    	float minScreenX = playerRect->x - (Constants::ScreenWidth_ /2);
    	float bX = bulletComponent->bullet.position.x;
    	//float bY = bulletComponent->bullet.position.y;
	    //std::cout << "updating bullet system: bullet.position.x" << std::endl;

	   // std::cout << "bullet position X " << bX << std::endl;
	   // std::cout << "maxScreenX " << maxScreenX << std::endl;
	   // std::cout << "minScreenX " << minScreenX << std::endl;
	    if (bX > Constants::ScreenWidth_ || bX < 0 || bX < minScreenX || bX > maxScreenX){
	    	aliveComponent->alive[eid] = false;
	    	std::cout << "BULLET OUT OF SCREEN" << std::endl;
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
}

