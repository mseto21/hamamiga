#include "AISystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "AIComponent.h"
#include "RectangleComponent.h"
#include "Rectangle.h"
#include "ComponentBag.h"

#include <SDL.h>
#include <iostream>

void AISystem_Initialize(AISystem* aiSystem, ComponentBag* cBag) {
  aiSystem->movementComponent   = cBag->movementComponent;
  aiSystem->rectangleComponent  = cBag->rectangleComponent;
  aiSystem->aiComponent         = cBag->aiComponent;
}

bool close(const Rectangle* r1, const Rectangle* r2) {
  if (abs((r1->x+r1->w)/2 - (r2->x+r2->w)/2) < Constants::Range_) {
    return true;
  }
  return false;
}

void AISystem_Update(AISystem* aiSystem) {
  RectangleComponent* rectangleComponent = aiSystem->rectangleComponent;
  MovementComponent* movementComponent = aiSystem->movementComponent;
  AIComponent* aiComponent = aiSystem->aiComponent;

  Rectangle pRect = rectangleComponent->entityRectangles[rectangleComponent->entityArray[Constants::PlayerIndex_]];
  pRect.x -= Constants::Range_;
  pRect.y += Constants::Range_;
  pRect.w += 2*Constants::Range_;
  pRect.h += 2*Constants::Range_;
  for (uint32 entityIndex = 0; entityIndex < aiComponent->count; entityIndex++) {
    uint32 eid = aiComponent->entityArray[entityIndex];
    if (!Component_HasIndex(movementComponent, eid)) {
      continue;
    }
    if (!Component_HasIndex(rectangleComponent, eid)) {
      continue;
    }
    MovementValues* moveValues = &movementComponent->movementValues[eid];
    Rectangle* eRect = &aiSystem->rectangleComponent->entityRectangles[eid];
    moveValues->xAccel = 0;
    if (close(&pRect, eRect)) {
      if ((pRect.x+Constants::Range_ + pRect.w/Constants::Range_)/2 < (eRect->x + eRect->w)/2) {
  	    moveValues->xAccel = -moveValues->accelX;
  	  } else {
  	    moveValues->xAccel = moveValues->accelX;
  	  }
    } else {
      MarchValues* marchValues = &aiComponent->marchValues[eid];
  	  moveValues->xAccel = moveValues->accelX*marchValues->facing;
  	  marchValues->distance += moveValues->xAccel*marchValues->facing;
  	  if (marchValues->distance >= marchValues->range) {
  	    marchValues->distance = 0;
  	    marchValues->facing *= -1;
  	    moveValues->xAccel *= -1;
  	  }
    }  
  }
}


