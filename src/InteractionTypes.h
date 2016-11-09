#pragma once
#include "Types.h"
struct ComponentBag;

enum InteractionTypes {
	InteractionType_Hat = 0,
	InteractionTypes_Dialogue,
	InteractionTypes_Cutscene,
	InteractionTypes_None
};

// ADD HATS AT END OF ENUM, DO NOT INSERT IN MIDDLE, NO! BAD!
enum HatTypes {
	HatTypes_BunnyHat = 0,	//0
	HatTypes_HardHat, 		//1
	HatTypes_Crown, 		//2
	HatTypes_Disco, 		//3
	HatTypes_Miner, 		//4
	HatTypes_Cowboy, 		//5
	HatTypes_Propeller,		//6
	HatTypes_Beer,      	//7

};

void ApplyHatInteraction(int hatType, uint32 eid, ComponentBag* cBag);
void PlayEventInteraction(uint32 eid, ComponentBag* cBag);