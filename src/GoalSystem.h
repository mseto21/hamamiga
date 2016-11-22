#pragma once
#include "Types.h"

struct ComponentBag;
struct HealthComponent;
struct AliveComponent;
struct GoalComponent;
struct RectangleComponent;
struct Zone;

enum GameResult {
	GameResult_Running = 0,
	GameResult_Killed,
	GameResult_Fell,
	GameResult_Won
};

struct GoalSystem {
	HealthComponent* healthComponent;
	AliveComponent* aliveComponent;
	GoalComponent* goalComponent;
	RectangleComponent* rectangleComponent;
	Zone* zone;
};

void GoalSystem_Initialize(GoalSystem* goalSystem, ComponentBag* cBag, Zone* zone);
GameResult GoalSystem_Update(GoalSystem* goalSystem, uint32 elapsed);
void GoalSystem_Free(GoalSystem* goalSystem);