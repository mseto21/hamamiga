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
	const float 	Second_ 		= 	1000.f;
	const float 	TargetFps_ 		= 	60.f;
	const float 	OptimalTime_ 	= 	Second_ / TargetFps_; // Update ~16 times per second

	const uint16	MaxMapWidth_	=	256; // In number of tiles
	const uint16	MaxMapHeight_	=	256;
	const uint16	TileSize_			=	48;
	const uint16	DiscoChannel_ 	= 	2;//channel for disco/overlay sounds
	const uint16  SoundChannel_ 	= 	3; //arbitrary channel for rest of sounds

	// Intro State
	const uint32	IntroTime_		= 	5000;

	// Title State
	const uint8	 	TitleScreenSelections_	=	4;

	// Zone Intro State Time
	const uint32	ZoneIntroTime_		=	3000;
	const uint32	CutSceneSlideTime_	=	5000;

	// Play State
	const float   	Gravity_        	=   .5f; //gravity
    const float   	Friction_   	 	=   .13f; //friction
    const float 	AirRes_				=   .05; //air resistance
	const uint16 	Damage_         	=   20; //damage
	const uint8		PlayerIndex_		=   0;
	const uint16    XRange_            	=   300;
	const uint16    YRange_            	=   168;


	// Hats sizes
	const uint16  	BunnyHatH_			=   52; 
	const uint16  	BunnyHatW_ 			=   48;
	const uint16  	ConstructionHatH_ 	=   24; 
	const uint16  	ConstructionHatW_	=   48;
	const uint16  	CrownH_				=   23; 
	const uint16  	CrownW_				=   48;
    const uint16  	DiscoHatH_			=   40;
    const uint16  	DiscoHatW_ 			=   48;

	// High Score State
	const int     MaxHighScores_		=		5;

	// Constant names
	const String128	TitleBackground_	= 	"title_background";
	const String128	TitleFader_			=	"title_fader";
	const String128	WinBackground_		= 	"win_background";
	const String128 LoseBackground_ 	=   "lose_background";
	const String128 GameBackground_ 	= 	"game_background";
	const String128 MenuOverlay_    	= 	"menu_overlay";
	const String128 BunnyHat_           =	"bunny";
	const String128 DiscoHat_           =	"disco";

}
