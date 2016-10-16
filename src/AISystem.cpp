#include "AISystem.h"
#include "Constants.h"
#include "Types.h"
#include "MovementComponent.h"
#include "AIComponent.h"

#include <SDL.h>
#include <iostream>

void AISystem_Update(AIComponent* aiComponent, MovementComponent* movementComponent, RectangleComponent* rectangleComponent) {
  for (uint32 entityIndex = 0; entityIndex < aiComponent->count; entityIndex++) {
    if (!Component_HasIndex(movementComponent, entityIndex)) {
	continue;
      }
      MovementValues* moveValues = &movementComponent->movementValues[movementComponent->entityArray[entityIndex]];
  }
}
