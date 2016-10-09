#pragma once
#include "Types.h"
#include <string>

namespace Constants {
	const uint16 	ScreenWidth_ 	= 	640;
	const uint16 	ScreenHeight_ = 	480;
	const float   	StepSize_ 		= 	8.f;
	const uint32  	NumKeys_			=		323;
	const uint32	MaxEntities_  =   128;
	const float 	TargetFps_ 		= 	60.f;
	const float 	OptimalTime_ 	= 	1000 / TargetFps_;

	// Intro State
	const float		IntroTime_		= 	250.f;

	// Title State
	const uint8	 	TitleScreenSelections_	=	4;

	// Play State
	const uint16 	PlayerWSize_ 	=	33; //player image size
	const uint16 	PlayerHSize_ 	=	60; //player image size
	const float    	Gravity_        =   0.5; //gravity
	const uint16    MaxV_           =   10; //max velocity
	const uint16  	Jump_           =   10; //jump velocity
    const float   	Accel_          =   7; //acceleration
	const uint16  	DemonHSize_   	=	60; 
  	const uint16  	DemonWSize_   	= 	61;// demon image size

	// High Score State
	const int     MaxHighScores_	=	5;

	// String names (TO-DO: Don't have them in here!)
	const String128	TitleBackground_	= 	"title_background";
	const String128	TitleFader_			=	"title_fader";
	const String128	WinBackground_		= 	"win_background";
	const String128 GameBackground_ 	= 	"game_background";
}
