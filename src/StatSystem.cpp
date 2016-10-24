#include "StatSystem.h"
#include "ComponentBag.h"
#include "HealthComponent.h"

void StatSystem_Initialize(StatSystem* statSystem, ComponentBag* cBag) {
	statSystem->healthComponent 	= cBag->healthComponent;
}


void StatSystem_Update(StatSystem* statSystem) {
        (void) statSystem;
	/*int* health = &game->playState.cBag.healthComponent->health[Constants::PlayerIndex_];
	int* y = &game->playState.cBag.rectangleComponent->entityRectangles[Constants::PlayerIndex_].y;
	if (*health <= 0 || *y >= Constants::LevelHeight_) {
	  game->gameState = GameState_Lose;
	}*/
}
