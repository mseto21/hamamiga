#include "AISystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "AIComponent.h"
#include "RectangleComponent.h"
#include "Rectangle.h"

#include <SDL.h>
#include <iostream>

bool close(const Rectangle* r1, const Rectangle* r2);

void AISystem_Update(AIComponent* aiComponent, MovementComponent* movementComponent, RectangleComponent* rectangleComponent, float timeStep) {
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
    Rectangle* eRect = &rectangleComponent->entityRectangles[eid];
    moveValues->xAccel = 0;
    if (close(&pRect, eRect)) {
      if ((pRect.x+Constants::Range_ + pRect.w/Constants::Range_)/2 < (eRect->x + eRect->w)/2) {
  	    moveValues->xAccel = -Constants::Accel_;
  	  } else {
  	    moveValues->xAccel = Constants::Accel_;
  	  }
  	} else {
  	  MarchValues* marchValues = &aiComponent->marchValues[eid];
  	  moveValues->xAccel = Constants::Accel_*marchValues->facing;
  	  marchValues->distance += moveValues->xAccel*timeStep*marchValues->facing;
  	  if (marchValues->distance >= marchValues->range) {
  	    marchValues->distance = 0;
  	    marchValues->facing *= -1;
  	    moveValues->xAccel *= -1;
  	  }
    }  
  }
}

bool close(const Rectangle* r1, const Rectangle* r2) {
  if (abs((r1->x+r1->w)/2 - (r2->x+r2->w)/2) < Constants::Range_) {
    std::cout << "in here" << std::endl;
    return true;
  }
  return false;
}
