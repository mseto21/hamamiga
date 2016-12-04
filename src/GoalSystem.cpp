#include "GoalSystem.h"
#include "StatSystem.h"
#include "ComponentBag.h"
#include "HealthComponent.h"
#include "AliveComponent.h"
#include "GoalComponent.h"
#include "RectangleComponent.h"
#include "Zone.h"

#include <iostream>

//constants for death score file
const char * deathpath = "assets/score/deaths.txt";
const char * levelspath = "assets/score/levels.txt";
const char * dval = "1";

void GoalSystem_Initialize(GoalSystem* goalSystem, ComponentBag* cBag, Zone* zone) {
	goalSystem->healthComponent 	= cBag->healthComponent;
	goalSystem->aliveComponent 		= cBag->aliveComponent;
	goalSystem->goalComponent 		= cBag->goalComponent;
	goalSystem->rectangleComponent  = cBag->rectangleComponent;
    goalSystem->zone                = zone;
}


GameResult GoalSystem_Update(GoalSystem* goalSystem, uint32 elapsed) {
	GoalComponent* goalComponent = goalSystem->goalComponent;
    HealthComponent* healthComponent = goalSystem->healthComponent;
    AliveComponent* aliveComponent = goalSystem->aliveComponent;
    RectangleComponent* rectangleComponent = goalSystem->rectangleComponent;

    for (uint32 entityIndex = 0; entityIndex < goalComponent->count; entityIndex++) {
    	uint32 eid = goalComponent->entityArray[entityIndex];
        if (!Component_HasIndex(goalComponent, eid)) {
            continue;
        }
    	if (Component_HasIndex(healthComponent, eid)) {
    		int health = healthComponent->health[eid];
			if (health <= 0) {
				if (Component_HasIndex(aliveComponent, eid)) {
				 	aliveComponent->alive[eid] = false;
				}
			 	if (eid == Constants::PlayerIndex_) {
                    Scores_Update(deathpath, (char*)"killed", dval);
				 	return GameResult_Killed;
				 }
			}
    	}

    	if (Component_HasIndex(rectangleComponent, eid)) {
    		if (rectangleComponent->entityRectangles[eid].y >= goalSystem->zone->levelHeight) {
    			if (Component_HasIndex(aliveComponent, eid)) {
				 	aliveComponent->alive[eid] = false;
				}
    			if (eid == Constants::PlayerIndex_) {
                    Scores_Update(deathpath, (char*)"fallen", dval);
	    			return GameResult_Fell;
	    		}
    		}
    	}

    	if (goalComponent->winGoal[eid]) {
    		if (eid == Constants::PlayerIndex_){
                Scores_Update(levelspath, (char*)"fallen", dval);
    			return GameResult_Won;
            }
    	}

    	goalComponent->points[eid] += elapsed;
	}	

	return GameResult_Running;
}

void GoalSystem_Free(GoalSystem* goalSystem) {
    goalSystem->healthComponent = nullptr;
    goalSystem->aliveComponent = nullptr;
    goalSystem->goalComponent = nullptr;
    goalSystem->rectangleComponent = nullptr;
}
