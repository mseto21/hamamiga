#include "FAISystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "FAIComponent.h"
#include "RectangleComponent.h"
#include "Rectangle.h"
#include "ComponentBag.h"

#include <SDL.h>
#include <iostream>

void FAISystem_Initialize(FAISystem* faiSystem, ComponentBag* cBag) {
  faiSystem->movementComponent   = cBag->movementComponent;
  faiSystem->rectangleComponent  = cBag->rectangleComponent;
  faiSystem->faiComponent         = cBag->faiComponent;
}

bool near(const Rectangle* r1, const Rectangle* r2) {
  if (abs(((int)(r1->x+(r1->w)/2)) - ((int)(r2->x+(r2->w)/2))) < Constants::XRange_
      && abs(((int)(r1->y + (r1->h)/2)) - ((int)(r2->y+(r2->h)/2))) < Constants::YRange_) {
    return true;
  }
  return false;
}

void FAISystem_Update(FAISystem* faiSystem) {
  RectangleComponent* rectangleComponent = faiSystem->rectangleComponent;
  MovementComponent* movementComponent = faiSystem->movementComponent;
  FAIComponent* faiComponent = faiSystem->faiComponent;

  Rectangle pRect = rectangleComponent->entityRectangles[rectangleComponent->entityArray[Constants::PlayerIndex_]];
  for (uint32 entityIndex = 0; entityIndex < faiComponent->count; entityIndex++) {

    uint32 eid = faiComponent->entityArray[entityIndex];
    if (!Component_HasIndex(faiComponent, eid)) {
      continue;
    }
    if (!Component_HasIndex(movementComponent, eid)) {
      continue;
    }
    if (!Component_HasIndex(rectangleComponent, eid)) {
      continue;
    }
    MovementValues* moveValues = &movementComponent->movementValues[eid];
    Rectangle* eRect = &faiSystem->rectangleComponent->entityRectangles[eid];
    moveValues->xAccel = 0;
    if (near(&pRect, eRect)) {
      if (pRect.x + (pRect.w)/2 < eRect->x + (eRect->w)/2) {
  	    moveValues->xAccel = -moveValues->accelX;
  	  } else {
  	    moveValues->xAccel = moveValues->accelX;
  	  }
      if (pRect.y + (pRect.h)/2 < eRect->x + (eRect->h)/2) {
	       moveValues->yAccel = -moveValues->accelY;
      } else {
	       moveValues->yAccel = moveValues->accelY;
      }
    } else {
      FlyValues* marchValues = &faiComponent->flyValues[eid];
  	  moveValues->yAccel = moveValues->accelY*marchValues->facing;
  	  marchValues->distance += moveValues->yAccel*marchValues->facing;
  	  if (marchValues->distance >= marchValues->range) {
  	    marchValues->distance = 0;
  	    marchValues->facing *= -1;
  	    moveValues->yAccel *= -1;
  	  }
    }  
  }
}

void FAISystem_Free(FAISystem* faiSystem) {
  faiSystem->movementComponent = nullptr;
  faiSystem->rectangleComponent = nullptr;
  faiSystem->faiComponent = nullptr;
}

