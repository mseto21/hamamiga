#pragma once
#include "Types.h"

struct ComponentBag;
struct HealthComponent;
struct AliveComponent;
struct GoalComponent;
struct RectangleComponent;

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
};

void GoalSystem_Initialize(GoalSystem* goalSystem, ComponentBag* cBag);
GameResult GoalSystem_Update(GoalSystem* goalSystem, uint32 elapsed);