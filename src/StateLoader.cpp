#include "Game.h"
#include "StateLoader.h"
#include "FileLoader.h"
#include "TextureCache.h"
#include "SoundCache.h"
#include "HealthComponent.h"

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

	TextureCache_CreateTexture(game->renderer, "assets/menu-screen.png", Constants::TitleBackground_);
	TextureCache_CreateTexture(game->renderer, "assets/menu-screen-overlay.png", Constants::MenuOverlay_);
	TextureCache_CreateTexture(game->renderer, "assets/fader.png", Constants::TitleFader_);
	TextureCache_CreateTexture(game->renderer, "assets/background.png", Constants::GameBackground_);
	TextureCache_CreateTexture(game->renderer, "assets/win-screen.png", Constants::WinBackground_);
	TextureCache_CreateTexture(game->renderer, "assets/lose-screen.png", Constants::LoseBackground_);
}



void LoadTitleStateAssets(Game* game) {
	// Load font textures
	game->titleState.selection = 0;
	game->titleState.titleFont = TTF_OpenFont("assets/minnie\'shat.ttf", 75);
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
		TextureCache_CreateFont(game->renderer, game->titleState.titleFont, baseColor, game->titleState.selectionStrings[selectionIndex], base.c_str());
		std::string select = game->titleState.selectionStrings[selectionIndex];
		select.append("_select");
		TextureCache_CreateFont(game->renderer, game->titleState.titleFont, selectedColor, game->titleState.selectionStrings[selectionIndex], select.c_str());
	}
	TTF_CloseFont(game->titleState.titleFont);

	// Load title music
	game->titleState.titleMusic = Mix_LoadMUS("assets/themesong.ogg");
	if (game->titleState.titleMusic == NULL) {
		std::cerr << "Unable to initialize titlescreen music! SDL_Error: " << Mix_GetError() << std::endl;
	}
}



void LoadHighScoreStateAssets(Game* game) {
	// Load font
	game->highScoreState.font = TTF_OpenFont("assets/minnie\'shat.ttf", 75);
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
		TextureCache_CreateFont(game->renderer, game->highScoreState.font, scoreColor, msg.c_str(), name.c_str());
	}
	TTF_CloseFont(game->highScoreState.font);
}



void LoadZoneIntroAssets(Game* game, String128 name) {
	game->zoneIntroState.alpha = 0.f;
	game->zoneIntroState.elapsed = 0;
	game->playState.chapter.startScene.current = 0;
	game->zoneIntroState.font = TTF_OpenFont("assets/BadMofo.ttf", 50);
	if (!game->zoneIntroState.font) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return;
	}

	SDL_Color color = {255, 255, 255, 255};
	TextureCache_CreateFont(game->renderer, game->zoneIntroState.font, color, name, "zone_name");
	TTF_CloseFont(game->zoneIntroState.font);
}



bool LoadPlayStateAssets(Game* game) {
	game->playState.scoreFont = TTF_OpenFont("assets/minnie\'shat.ttf", 30);
	game->playState.healthFont = TTF_OpenFont("assets/minnie\'shat.ttf", 30);
	TextureCache_CreateTexture(game->renderer, "assets/bunny-hat.png", Constants::BunnyHat_);
	TextureCache_CreateTexture(game->renderer, "assets/disco-hat.png", Constants::DiscoHat_);
	if (!game->playState.scoreFont) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return false;
	}
	if (!game->playState.healthFont) {
	    std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
	    return false;
	}
	//Creating all sounds for the play state
	SoundCache_CreateSound("assets/sounds/footsteps.ogg", "walking");
	SoundCache_CreateSound("assets/sounds/hatpickup.ogg", "hatpickup");
	SoundCache_CreateSound("assets/sounds/disco.ogg", "disco");
	
	TTF_SetFontHinting(game->playState.scoreFont, TTF_HINTING_MONO);
	TTF_SetFontHinting(game->playState.healthFont, TTF_HINTING_MONO);
	ComponentBag_Malloc(&game->playState.cBag);

	// TO-DO: Hardcoded for now, but its coolio!
	FileLoader_Load(&game->playState.chapter, "assets/chapter_1/chapter_1.txt", &game->playState.cBag, game->renderer); // Hardcoded for now, but easily an array.
	if (!Component_HasIndex(game->playState.cBag.healthComponent, Constants::PlayerIndex_)) {
		std::cerr << "Error: The player has no renderable health component" << std::endl;
		//return;
	}

	AISystem_Initialize(&game->playState.aiSystem, &game->playState.cBag);
	CameraSystem_Initialize(&game->playState.cameraSystem, &game->playState.cBag);
	InputSystem_Initialize(&game->playState.inputSystem, &game->playState.cBag);
	MovementSystem_Initialize(&game->playState.movementSystem, &game->playState.cBag);
	PhysicsSystem_Initialize(&game->playState.physicsSystem, &game->playState.cBag,  &game->playState.chapter.tileMap);
	RenderSystem_Initialize(&game->playState.renderSystem, &game->playState.cBag, &game->playState.chapter.tileMap);
	StatSystem_Initialize(&game->playState.statSystem, &game->playState.cBag);
	SoundSystem_Initialize(&game->playState.soundSystem, &game->playState.cBag, game->playState.chapter.music);
	return true;
}
