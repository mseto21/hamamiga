#pragma once
#include "Types.h"

namespace Constants {
	const uint16 	ScreenHeight_ 	= 	480;
	const uint16 	ScreenWidth_ 	= 	640;
	const uint16 	PlayerWSize_ 	=	67; //player image size
	const uint16 	PlayerHSize_ 	=	41; //player image size
	const float   	StepSize_ 		= 	8.f;
	const uint32    NumKeys_		=	323;
	const uint32	MaxEntities_  	=   128;
	const float 	TargetFps_ = 60.f;
	const float 	OptimalTime_ = 1000 / TargetFps_;
}