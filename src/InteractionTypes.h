#pragma once
#include "Types.h"
struct ComponentBag;
struct Game;
struct Texture;

enum InteractionTypes {
	InteractionType_Hat = 0,
	InteractionTypes_Dialogue,
	InteractionTypes_Cutscene,
	InteractionTypes_None
};

enum HatTypes {
	HatTypes_BunnyHat = 0,	//0
	HatTypes_HardHat, 		//1
	HatTypes_Crown, 		//2
	HatTypes_Disco, 		//3
	HatTypes_Miner, 		//4
	HatTypes_Cowboy, 		//5
	HatTypes_Propeller,		//6
	HatTypes_Beer,      	//7
	HatTypes_Chef,		//8
	HatTypes_Empty,			// Leave at end please.
};

bool Interaction_ApplyHatInteraction(int hatType, uint32 eid, uint32 hatEid, ComponentBag* cBag);
void Interaction_RemoveHatInteraction(uint32 eid, ComponentBag* cBag);
void Interaction_DisplayMessage(Game* game, Texture** txt);
void Interaction_PlayEventInteraction(uint32 eid, ComponentBag* cBag);