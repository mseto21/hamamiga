#pragma once
#include "Types.h"

namespace Constants {
	const uint16 	ScreenWidth_ 	= 	1080;
	const uint16 	ScreenHeight_ 	= 	761;
	const uint16 	LevelWidth_ 	= 	5040;
	const uint16 	LevelHeight_ 	= 	1470;
	const float   	StepSize_ 		= 	8.f;
	const uint32  	NumKeys_		=	323;
	const uint32	MaxEntities_  	=   128;
	const float 	TargetFps_ 		= 	60.f;
	const float 	OptimalTime_ 	= 	1000 / TargetFps_;

	const uint16	MaxMapWidth_	=	256; // In number of tiles
	const uint16	MaxMapHeight_	=	256;
	const uint16	TileSize_		=	48;

	// Intro State
	const float		IntroTime_		= 	250.f;

	// Title State
	const uint8	 	TitleScreenSelections_	=	4;

	// Play State
	const float   	Gravity_        	=   0.75; //gravity
    const float   	Friction_   	 	=   .15; //friction
	const uint16  	Jump_           	=   20; //jump velocity
	const uint16 	Damage_         	=   20; //damage
	const uint8		PlayerIndex_		=	0;
	const uint16    Range_              =   200;

	// Hats sizes
	const uint16  	BunnyHatH_          	=   52; 
	const uint16  	BunnyHatW_          	=   38;
	const uint16  	ConstructionHatH_       =   24; 
	const uint16  	ConstructionHatW_       =   48;
	const uint16  	CrownH_          	    =   23; 
	const uint16  	CrownW_          	    =   43;

	// High Score State
	const int     MaxHighScores_			=		5;

	// String names (TO-DO: Don't have them in here!)
	const String128	TitleBackground_	= 	"title_background";
	const String128	TitleFader_			=	"title_fader";
	const String128	WinBackground_		= 	"win_background";
	const String128 LoseBackground_ 	=   "lose_background";
	const String128 GameBackground_ 	= 	"game_background";
	const String128 MenuOverlay_    	= 	"menu_overlay";
}
