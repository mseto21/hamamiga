#include "AISystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "AIComponent.h"
#include "RectangleComponent.h"
#include "AnimationComponent.h"
#include "PhysicsComponent.h"
#include "Rectangle.h"
#include "ComponentBag.h"

#include <SDL.h>
#include <iostream>

const float SpeedMultiplier_ = 1.25;

void AISystem_Initialize(AISystem* aiSystem, ComponentBag* cBag) {
  aiSystem->movementComponent   = cBag->movementComponent;
  aiSystem->rectangleComponent  = cBag->rectangleComponent;
  aiSystem->aiComponent         = cBag->aiComponent;
  aiSystem->animationComponent  = cBag->animationComponent;
  aiSystem->rectangleComponent  = cBag->rectangleComponent;
  aiSystem->physicsComponent    = cBag->physicsComponent;
}

bool close(const Rectangle* r1, const Rectangle* r2) {
  if (abs(((int)(r1->x+(r1->w)/2)) - ((int)(r2->x+(r2->w)/2))) < Constants::XRange_
      && abs(((int)(r1->y + (r1->h)/2)) - ((int)(r2->y+(r2->h)/2))) < Constants::YRange_) {
    return true;
  }
  return false;
}


void MarcherUpdate(AISystem* aiSystem, uint32 eid) {
  AIComponent* aiComponent = aiSystem->aiComponent;
  AnimationComponent* animationComponent = aiSystem->animationComponent;
  RectangleComponent* rectangleComponent = aiSystem->rectangleComponent;
  MovementComponent* movementComponent = aiSystem->movementComponent;
  if (!Component_HasIndex(movementComponent, eid)) {
    return;
  }
  if (!Component_HasIndex(rectangleComponent, eid)) {
    return;
  }

  Rectangle pRect = rectangleComponent->entityRectangles[rectangleComponent->entityArray[Constants::PlayerIndex_]];

  MovementValues* moveValues = &movementComponent->movementValues[eid];
  moveValues->xAccel = 0;
  MarchValues* marchValues = &aiComponent->marchValues[eid];
  Rectangle* eRect = &rectangleComponent->entityRectangles[eid];
  if (close(&pRect, eRect)) {
    if (!marchValues->aggrod) {
      animationComponent->animations[eid].frameTime /= SpeedMultiplier_;
      moveValues->maxXVelocity *= SpeedMultiplier_;
      marchValues->aggrod = true;
    }
    if (pRect.x + (pRect.w)/2 < eRect->x + (eRect->w)/2) {
      moveValues->xAccel = -moveValues->accelX;
    } else {
      moveValues->xAccel = moveValues->accelX;
    }
  } else {
    if (marchValues->aggrod) {
      animationComponent->animations[eid].frameTime *= SpeedMultiplier_;
      moveValues->maxXVelocity /= SpeedMultiplier_;
      marchValues->aggrod = false;
    }
    moveValues->xAccel = moveValues->accelX*marchValues->facing;
    marchValues->distance += moveValues->xAccel*marchValues->facing;
    if (marchValues->distance >= marchValues->range) {
      marchValues->distance = 0;
      marchValues->facing *= -1;
      moveValues->xAccel *= -1;
    }
  } 
}


void FlyerUpdate(AISystem* aiSystem, uint32 eid) {
  AIComponent* aiComponent = aiSystem->aiComponent;
  MovementComponent* movementComponent = aiSystem->movementComponent;
  RectangleComponent* rectangleComponent = aiSystem->rectangleComponent;
  if (!Component_HasIndex(movementComponent, eid)) {
    return;
  }
  if (!Component_HasIndex(rectangleComponent, eid)) {
    return;
  }
  Rectangle pRect = rectangleComponent->entityRectangles[rectangleComponent->entityArray[Constants::PlayerIndex_]];
  MovementValues* moveValues = &movementComponent->movementValues[eid];
  Rectangle* eRect = &rectangleComponent->entityRectangles[eid];
  moveValues->xAccel = 0;
  moveValues->yAccel = 0;
  if (close(&pRect, eRect)) {
    if (pRect.x + (pRect.w)/2 < eRect->x + (eRect->w)/2) {
      moveValues->xAccel = -moveValues->accelX;
    } else {
      moveValues->xAccel = moveValues->accelX;
    }
    if (pRect.y + (pRect.h)/2 < eRect->y + (eRect->h)/2) {
      moveValues->yAccel = -moveValues->accelY;
    } else {
      moveValues->yAccel = moveValues->accelY;
    }
  } else {
      MarchValues* marchValues = &aiComponent->marchValues[eid];
      moveValues->yAccel = moveValues->accelY*marchValues->facing;
      marchValues->distance += moveValues->yAccel*marchValues->facing;
      if (marchValues->distance >= marchValues->range) {
        marchValues->distance = 0;
        marchValues->facing *= -1;
        moveValues->xAccel *= -1;
      }
  } 
}


void ProjectileUpdate(AISystem* aiSystem, uint32 eid) {
  MovementComponent* movementComponent = aiSystem->movementComponent;
  PhysicsComponent* physicsComponent = aiSystem->physicsComponent;
  if (!Component_HasIndex(movementComponent, eid)) {
    return;
  }

  if (!physicsComponent->physicsValues[eid].collided) {
    MovementValues* moveValues = &movementComponent->movementValues[eid];
    moveValues->xVelocity = moveValues->maxXVelocity;
    moveValues->xAccel = moveValues->accelX;
  }
}


void ThrowerUpdate(AISystem* aiSystem, uint32 eid) {
  (void) aiSystem;
  (void) eid;
}


void JumperUpdate(AISystem* aiSystem, uint32 eid) {
  AIComponent* aiComponent = aiSystem->aiComponent;
  RectangleComponent* rectangleComponent = aiSystem->rectangleComponent;
  MovementComponent* movementComponent = aiSystem->movementComponent;

  if (!Component_HasIndex(movementComponent, eid)) {
    return;
  }

  Rectangle pRect = rectangleComponent->entityRectangles[Constants::PlayerIndex_];
  Rectangle eRect = rectangleComponent->entityRectangles[eid];
  MovementValues* moveValues = &movementComponent->movementValues[eid];
  if (close(&pRect, &eRect))
    aiComponent->marchValues[eid].facing = (pRect.x > eRect.x) - (pRect.x < eRect.x);

  if (moveValues->grounded)
    moveValues->yAccel = -moveValues->accelY;
  else
    moveValues->yAccel = 0;
}


void AISystem_Update(AISystem* aiSystem) {
  AIComponent* aiComponent = aiSystem->aiComponent;
  for (uint32 entityIndex = 0; entityIndex < aiComponent->count; entityIndex++) {
    uint32 eid = aiComponent->entityArray[entityIndex];
    switch (aiComponent->types[eid]) {
      case AIType_Marcher:
        MarcherUpdate(aiSystem, eid);
        break;
      case AIType_Projectile:
        ProjectileUpdate(aiSystem, eid);
        break;
      case AIType_Thrower:
        ThrowerUpdate(aiSystem, eid);
        break;
      case AIType_Flyer:
        FlyerUpdate(aiSystem, eid);
        break;
      case AIType_Jumper:
        JumperUpdate(aiSystem, eid);
        break;
    } 
  }
}

void AISystem_Free(AISystem* aiSystem) {
  aiSystem->movementComponent = nullptr;
  aiSystem->aiComponent = nullptr;
}

