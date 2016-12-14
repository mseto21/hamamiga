#pragma once
#include "Types.h"

namespace Constants {
	const uint16 	ScreenWidth_ 	= 	1080;
	const uint16 	ScreenHeight_ 	= 	760;

	const float   	StepSize_ 			= 	8.f;
	const uint32  	NumKeys_			=	323;
	const uint32	MaxEntities_  		=   128;
	const float 	Second_ 			= 	1000.f;
	const float 	TargetFps_ 			= 	60.f;
	const float 	OptimalTime_ 		= 	Second_ / TargetFps_; // Update ~16 times per second

	// Intro State
	const uint32	IntroTime_			= 	5000;

	// Title State
	const uint8	 	TitleScreenSelections_	=	5;
	const uint8	 	TitleScreenTextures_	=	6;
	
	// Option State
	const uint8	 	OptionScreenSelections_	=	1;
	const uint8		VolumeUnit_				=	1;
	const uint8		MaxVolume_				= 	128;
	const float		BrightnessUnit_			= 	0.1f;
	const float 	MaxBrightness_			= 	1.0f;

	// Zone Intro State Time
	const uint32	ZoneIntroTime_		=	3000;
	const uint32	FaderTime_			=	500;
	const uint32	SlideTime_			= 	300;
	const uint32	SlideOvershoot_ 	=	ScreenWidth_ / 8;
	const uint32	OvershootTime_		=	150;

	// Play State
	const float   	Gravity_        	= .5f; //gravity
  	const float   	Friction_   	 	= .13f; //friction
  	const float 	AirRes_				= .05; //air resistance
	const uint16 	Damage_         	= 20; //damage
	const uint8		PlayerIndex_		= 0;
	const uint16  	XRange_           	= 300;
	const uint16  	YRange_           	= 1;
	const uint8 	MaximumLevels_ 		= 4;
	const uint16	MaxMapWidth_		= 256; // In number of tiles
	const uint16	MaxMapHeight_		= 256;
	const uint16	TileSize_			= 48;
	const uint16	MaxTileAnimations_	= 8;
	const uint16	DiscoChannel_		= 2; // Channel for disco/overlay sounds
	const uint16  	SoundChannel_		= 3; // Arbitrary channel for rest of sounds
	const uint16	MaxBullets_			= 3; // Max number of alive bullets at a time
	const uint16    MaxKnives_			= 3; // Max number of alive knives at a time
	const uint8		CoinValue_			= 5; // Points allocated for one coin.
	const uint16	InvincibleTime_		= 3000;

	// Pause State
	const uint16	MaxInteractableMessages_	= 12;
	const int		DialogSize_					= ScreenWidth_ / 4;

	// High Score State
	const int     MaxHighScores_			=		6;

	// Constant names
	const String128	TitleBackground_	= 	"title_background";
	const String128	TitleFader_			=	"title_fader";
	const String128	WinBackground_		= 	"win_background";
	const String128 LoseBackground_ 	=   "lose_background";
	const String128 MenuOverlay_    	= 	"menu_overlay";
	const String128 Shader_    			= 	"shader";
	const String128 PShader_			=	"pShader";
	const String128 GameBackground_ 	= 	"game_background";
	const String128 TilesetName_ 		= 	"tileset";
	const String128 ZoneName_ 			=	"zone_name";
	const String128 HealthBar_			= 	"health_bar";
	const String128 MainMenuO_		=      "main_menu_overlay";
	const String128 StatsO_				=	"stats_overlay";
	const String128 LevelsO_			= 	"levels_overlay";
	const String128 OptionsO_			= 	"options_overlay";
	const String128 SoundBar_			=	"sound_bar";
	const String128 TimeBar_			= 	"time_bar";
	const String128 CoinBar_			= 	"coin_bar";
	const String128 GlamourGlow_		= 	"glamour_glow";
	const String128 HatGlow_			= 	"hat_glow";
}
