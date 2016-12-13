#include "GoalComponent.h"

void GoalComponent_Add(GoalComponent* goalComponent, uint32 eid) {
	Component_Add(goalComponent, eid);
	goalComponent->winGoal[eid] = false;
	goalComponent->points[eid] = 0;
}
