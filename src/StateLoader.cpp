#include "Game.h"
#include "StateLoader.h"
#include "FileLoader.h"
#include "TextureCache.h"
#include "SoundCache.h"
#include "HealthComponent.h"
#include "EntityCache.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>


void LoadIntroStateAssets(Game* game) {
	game->introState.elapsed = 0;
	game->introState.alpha = 0.f;

	TextureCache_CreateTexture(game->renderer, "assets/screens/menu-screen.png", Constants::TitleBackground_);
	TextureCache_CreateTexture(game->renderer, "assets/screens/menu-screen-overlay.png", Constants::MenuOverlay_);
	TextureCache_CreateTexture(game->renderer, "assets/screens/fader.png", Constants::TitleFader_);
	TextureCache_CreateTexture(game->renderer, "assets/screens/win-screen.png", Constants::WinBackground_);
	TextureCache_CreateTexture(game->renderer, "assets/screens/lose-screen.png", Constants::LoseBackground_);
}



void LoadTitleStateAssets(Game* game) {
	// Load font textures
	game->titleState.selection = 0;
	game->titleState.titleFont = TTF_OpenFont("assets/fonts/minnie\'shat.ttf", 75);
	if (!game->titleState.titleFont) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return;
	}
	TTF_SetFontHinting(game->titleState.titleFont, TTF_HINTING_MONO);
	game->titleState.selectionStrings[0] = "Play";
	game->titleState.selectionStrings[1] = "High Scores";
	game->titleState.selectionStrings[2] = "Options";
	game->titleState.selectionStrings[3] = "Quit";

	SDL_Color baseColor = {255, 255, 255, 255};
	SDL_Color selectedColor = {100, 100, 100, 100};
	for (int selectionIndex = 0; selectionIndex < Constants::TitleScreenSelections_; selectionIndex++) {
		std::string base = game->titleState.selectionStrings[selectionIndex];
		base.append("_base");
		TextureCache_CreateTextureFromFont(game->renderer, game->titleState.titleFont, baseColor, game->titleState.selectionStrings[selectionIndex], base.c_str());
		std::string select = game->titleState.selectionStrings[selectionIndex];
		select.append("_select");
		TextureCache_CreateTextureFromFont(game->renderer, game->titleState.titleFont, selectedColor, game->titleState.selectionStrings[selectionIndex], select.c_str());
	}
	TTF_CloseFont(game->titleState.titleFont);

	// Load title music
	game->titleState.titleMusic = Mix_LoadMUS("assets/music/themesong.ogg");
	if (game->titleState.titleMusic == NULL) {
		std::cerr << "Unable to initialize titlescreen music! SDL_Error: " << Mix_GetError() << std::endl;
	}
}



void LoadHighScoreStateAssets(Game* game) {
	// Load font
	game->highScoreState.font = TTF_OpenFont("assets/fonts/minnie\'shat.ttf", 75);
	if (!game->highScoreState.font) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return;
	}
	// TO-DO: Read this in from a file.
	// Create textures for the current high scores
	SDL_Color scoreColor = {255, 255, 255, 255};
	for (int highScoreIndex = 0; highScoreIndex < Constants::MaxHighScores_; highScoreIndex++) {
		std::string msg = std::to_string(game->highScoreState.scores[highScoreIndex]);
		std::string name = "high_score_";
		name.append(std::to_string(game->highScoreState.scores[highScoreIndex]));
		TextureCache_CreateTextureFromFont(game->renderer, game->highScoreState.font, scoreColor, msg.c_str(), name.c_str());
	}
	TTF_CloseFont(game->highScoreState.font);
}


void LoadOptionStateAssets(Game* game) {
	// Load font
	game->optionState.font = TTF_OpenFont("assets/fonts/minnie\'shat.ttf", 75);
	if (!game->optionState.font) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return;
	}
	//Game options
	TTF_SetFontHinting(game->optionState.font, TTF_HINTING_MONO);
	game->optionState.selectionStrings[0] = "Background Music";
	game->optionState.selectionStrings[1] = "Brightness";
	//game->optionState.selectionStrings[2] = "Options";
	//game->optionState.selectionStrings[3] = "Quit";

	SDL_Color baseColor = {255, 255, 255, 255};
	SDL_Color selectedColor = {100, 100, 100, 100};
	for (int selectionIndex = 0; selectionIndex < Constants::OptionScreenSelections_; selectionIndex++) {
		std::string base = game->optionState.selectionStrings[selectionIndex];
		base.append("_base");
		TextureCache_CreateTextureFromFont(game->renderer, game->optionState.font, baseColor, game->optionState.selectionStrings[selectionIndex], base.c_str());
		std::string select = game->optionState.selectionStrings[selectionIndex];
		select.append("_select");
		TextureCache_CreateTextureFromFont(game->renderer, game->optionState.font, selectedColor, game->optionState.selectionStrings[selectionIndex], select.c_str());
	}
	TTF_CloseFont(game->highScoreState.font);
}



void LoadZoneIntroAssets(Game* game, String128 name) {
	game->zoneIntroState.alpha = 0.f;
	game->zoneIntroState.elapsed = 0;
	game->zoneIntroState.startScene.current = 0;
	game->zoneIntroState.font = TTF_OpenFont("assets/fonts/BadMofo.ttf", 50);
	if (!game->zoneIntroState.font) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return;
	}

	SDL_Color color = {255, 255, 255, 255};
	TextureCache_CreateTextureFromFont(game->renderer, game->zoneIntroState.font, color, name, Constants::ZoneName_);
	TTF_CloseFont(game->zoneIntroState.font);
}



bool LoadPlayStateAssets(Game* game, int chapter) {
	// Initialize caches
	TextureCache* tcache = TextureCache_GetCache();
	tcache->levelIndex = tcache->index;
	if (EntityCache_GetCache() == NULL) {
		EntityCache_Free();
		std::cerr << "Error: The entity cache was already loaded!" << std::endl;
		return false;
	}
	ComponentBag_Malloc(&game->playState.cBag);

	// Initialize file
	std::string chapterPath = "assets/chapter_" + std::to_string(chapter) + "/chapter_" + std::to_string(chapter)  + ".txt";
	std::string backgroundPath = "assets/chapter_" + std::to_string(chapter) + "/background.png";
	TextureCache_CreateTexture(game->renderer, backgroundPath.c_str(), Constants::GameBackground_);
	std::string shaderPath = "assets/chapter_" + std::to_string(chapter) + "/shader.png";
	SDL_SetTextureBlendMode(TextureCache_CreateTexture(game->renderer, shaderPath.c_str(), Constants::Shader_)->sdltexture, SDL_BLENDMODE_MOD);
	std::string pShaderPath = "assets/chapter_" + std::to_string(chapter) + "/player-shader.png";
	SDL_SetTextureBlendMode(TextureCache_CreateTexture(game->renderer, pShaderPath.c_str(), Constants::PShader_)->sdltexture, SDL_BLENDMODE_MOD);
	
	// Load file
	if (!FileLoader_Load(&game->playState.chapter, chapterPath.c_str(), &game->playState.cBag, game->renderer, &game->zoneIntroState)) {
		EntityCache_Free();
		ComponentBag_Free(&game->playState.cBag);
		std::cerr << "Error: Unable to load from path " << chapterPath << std::endl;
		return false;
	}
	
	// Ensure health component is present
	if (!Component_HasIndex(game->playState.cBag.healthComponent, Constants::PlayerIndex_)) {
		EntityCache_Free();
		ComponentBag_Free(&game->playState.cBag);
		std::cerr << "Error: The player has no renderable health component" << std::endl;
		return false;
	}

	// Initialize fonts
	game->playState.scoreFont = TTF_OpenFont("assets/fonts/minnie\'shat.ttf", 30);
	game->playState.healthFont = TTF_OpenFont("assets/fonts/minnie\'shat.ttf", 30);
	game->zoneIntroState.sliding = false;
	if (!game->playState.scoreFont) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
	}
	if (!game->playState.healthFont) {
	    std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
	}

	//Create Sounds
	SoundCache_CreateSound("assets/sounds/hatpickup.ogg", "hatpickup");
	SoundCache_CreateSound("assets/sounds/hop_soundbible.ogg", "hop");
	SoundCache_CreateSound("assets/sounds/disco.ogg", "disco");
	SoundCache_CreateSound("assets/sounds/western.ogg", "western");
	SoundCache_CreateSound("assets/sounds/ow.ogg", "ow");
	TextureCache_CreateTexture(game->renderer, "assets/hats/bullet.png", "bullet");
	SDL_SetTextureBlendMode(TextureCache_CreateTexture(game->renderer, "assets/hats/miner-shader.png", "miner-shader")->sdltexture, SDL_BLENDMODE_MOD);
	TTF_SetFontHinting(game->playState.scoreFont, TTF_HINTING_MONO);
	TTF_SetFontHinting(game->playState.healthFont, TTF_HINTING_MONO);

	// Initialize systems
	AISystem_Initialize(&game->playState.aiSystem, &game->playState.cBag);
	FAISystem_Initialize(&game->playState.faiSystem, &game->playState.cBag);
	CameraSystem_Initialize(&game->playState.cameraSystem, &game->playState.cBag);
	InputSystem_Initialize(&game->playState.inputSystem, &game->playState.cBag);
	MovementSystem_Initialize(&game->playState.movementSystem, &game->playState.cBag);
	PhysicsSystem_Initialize(&game->playState.physicsSystem, &game->playState.cBag,  &game->playState.chapter.tileMap, game);
	RenderSystem_Initialize(&game->playState.renderSystem, &game->playState.cBag, &game->playState.chapter.tileMap, game->playState.scoreFont);
	GoalSystem_Initialize(&game->playState.goalSystem, &game->playState.cBag);
	SoundSystem_Initialize(&game->playState.soundSystem, &game->playState.cBag, game->playState.chapter.music);
	BulletSystem_Initialize(&game->playState.bulletSystem, &game->playState.cBag);
	KillSystem_Initialize(&game->playState.killSystem, &game->playState.cBag);

	// Pause state
	TextureCache_CreateTexture(game->renderer, "assets/interactables/speech-bubble.png", "speech-bubble");

	// Set loaded variable
	game->playState.loaded = true;

	return true;
}


void FreePlay(Game* game) {
	// Free sounds
	//Mix_FreeMusic(game->playState.chapter.music);
	//game->playState.chapter.music = nullptr;
	Mix_HaltChannel(Constants::DiscoChannel_);
	// Free caches
	EntityCache_Free();
	ComponentBag_Free(&game->playState.cBag);
	TextureCache_FreeLevel();

	// Free cutscenes
	strcpy(game->playState.chapter.name, "");
	//for (int i = 0; i < game->playState.chapter.startScene.slideCount; i++) {
	//	game->playState.chapter.startScene.slides[i] = nullptr;
	//}
	game->zoneIntroState.startScene.slideCount = 0;
	game->zoneIntroState.startScene.current = 0;

	//for (int i = 0; i < game->playState.chapter.endScene.slideCount; i++) {
	//	game->playState.chapter.endScene.slides[i] = nullptr;
	//}
	game->zoneIntroState.endScene.slideCount = 0;
	game->zoneIntroState.endScene.current = 0;

	// Free fonts
	TTF_CloseFont(game->playState.scoreFont);
	TTF_CloseFont(game->playState.healthFont);

	// Free sounds
	//SoundCache_FreeSound("hatpickup");
	////SoundCache_FreeSound("disco");
	//SoundCache_FreeSound("ow");
	//SoundCache_FreeSound("nj");

	// Delete all pointers in ai system
	AISystem_Free(&game->playState.aiSystem);
	FAISystem_Free(&game->playState.faiSystem);
	CameraSystem_Free(&game->playState.cameraSystem);
	InputSystem_Free(&game->playState.inputSystem);
	MovementSystem_Free(&game->playState.movementSystem);
	PhysicsSystem_Free(&game->playState.physicsSystem);
	RenderSystem_Free(&game->playState.renderSystem);
	GoalSystem_Free(&game->playState.goalSystem);
//	SoundSystem_Free(&game->playState.soundSystem);
	BulletSystem_Free(&game->playState.bulletSystem);
	KillSystem_Free(&game->playState.killSystem);

	// Set loaded variable.
	game->playState.loaded = false;
}




