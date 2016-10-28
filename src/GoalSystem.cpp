#include "GoalSystem.h"
#include "ComponentBag.h"
#include "HealthComponent.h"
#include "AliveComponent.h"
#include "GoalComponent.h"
#include "RectangleComponent.h"

#include <iostream>

void GoalSystem_Initialize(GoalSystem* goalSystem, ComponentBag* cBag) {
	goalSystem->healthComponent 	= cBag->healthComponent;
	goalSystem->aliveComponent 		= cBag->aliveComponent;
	goalSystem->goalComponent 		= cBag->goalComponent;
	goalSystem->rectangleComponent = cBag->rectangleComponent;
}


GameResult GoalSystem_Update(GoalSystem* goalSystem, uint32 elapsed) {
		GoalComponent* goalComponent = goalSystem->goalComponent;
    HealthComponent* healthComponent = goalSystem->healthComponent;
    AliveComponent* aliveComponent = goalSystem->aliveComponent;
    RectangleComponent* rectangleComponent = goalSystem->rectangleComponent;

    for (uint32 entityIndex = 0; entityIndex < goalComponent->count; entityIndex++) {
    	uint32 eid = goalComponent->entityArray[entityIndex];

    	if (Component_HasIndex(healthComponent, eid)) {
    		int health = healthComponent->health[eid];
				if (health <= 0) {
					if (Component_HasIndex(aliveComponent, eid)) {
					 	aliveComponent->alive[eid] = false;
					}
				 	if (eid == Constants::PlayerIndex_) {
					 	return GameResult_Killed;
					 }
				}
    	}

    	if (Component_HasIndex(rectangleComponent, eid)) {
    		if (rectangleComponent->entityRectangles[eid].y >= Constants::LevelHeight_) {
    			if (Component_HasIndex(aliveComponent, eid)) {
				 	aliveComponent->alive[eid] = false;
				}
    			if (eid == Constants::PlayerIndex_) {
	    			return GameResult_Fell;
	    		}
    		}
    	}

    	if (goalComponent->winGoal[eid]) {
    		if (eid == Constants::PlayerIndex_)
    			return GameResult_Won;
    	}

    	goalComponent->points[eid] += elapsed;
    	
	}	

	return GameResult_Running;
}
