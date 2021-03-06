#include "Game.h"
#include "StateLoader.h"
#include "StatSystem.h"
#include "FileLoader.h"
#include "TextureCache.h"
#include "SoundCache.h"
#include "HealthComponent.h"
#include "InteractableComponent.h"
#include "EntityCache.h"
#include "Interactions.h"
#include "Constants.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>


// TO-DO: This is temporary until we can save files!
std::string LevelNames[] = {
	"Tutorial",
	"The Mysterious, Murderous Memo",
	"Into the Vents!",
	"Scurry Out of Shipping",
	"Breakroom Brawl",
	"Credits",
};


void LoadIntroStateAssets(Game* game) {
	game->introState.elapsed = 0;
	game->introState.alpha = 0.f;

	TextureCache_CreateTexture(game->renderer, "assets/screens/menu-screen.png", Constants::TitleBackground_);
	TextureCache_CreateTexture(game->renderer, "assets/screens/menu-screen-overlay.png", Constants::MenuOverlay_);
	TextureCache_CreateTexture(game->renderer, "assets/screens/fader.png", Constants::TitleFader_);
	TextureCache_CreateTexture(game->renderer, "assets/screens/win-screen.png", Constants::WinBackground_);
	TextureCache_CreateTexture(game->renderer, "assets/screens/lose-screen.png", Constants::LoseBackground_);
	TextureCache_CreateTexture(game->renderer, "assets/menu-overlays/main-menu.png", Constants::MainMenuO_);
	TextureCache_CreateTexture(game->renderer, "assets/menu-overlays/levels.png", Constants::LevelsO_);
	TextureCache_CreateTexture(game->renderer, "assets/menu-overlays/stats.png", Constants::StatsO_);
	TextureCache_CreateTexture(game->renderer, "assets/menu-overlays/options.png", Constants::OptionsO_);
}



void LoadTitleStateAssets(Game* game) {
	// Load font textures
	game->titleState.selection = 0;
	TTF_Font* font = TTF_OpenFont("assets/fonts/minnie\'shat.ttf", 75);
	if (!font) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return;
	}
	TTF_SetFontHinting(font, TTF_HINTING_MONO);
	game->titleState.selectionStrings[0] = "New Game";
	game->titleState.selectionStrings[1] = "Controls";
	game->titleState.selectionStrings[2] = "Options";
	game->titleState.selectionStrings[3] = "Game Stats";
	game->titleState.selectionStrings[4] = "Quit";
	game->titleState.selectionStrings[5] = "Continue...";

	SDL_Color baseColor = {255, 255, 255, 255};
	SDL_Color selectedColor = {100, 100, 100, 100};
	for (int selectionIndex = 0; selectionIndex < Constants::TitleScreenTextures_; selectionIndex++) {
		std::string base = game->titleState.selectionStrings[selectionIndex];
		base.append("_base");
		TextureCache_CreateTextureFromFont(game->renderer, font, baseColor, game->titleState.selectionStrings[selectionIndex], base.c_str());
		std::string select = game->titleState.selectionStrings[selectionIndex];
		select.append("_select");
		TextureCache_CreateTextureFromFont(game->renderer, font, selectedColor, game->titleState.selectionStrings[selectionIndex], select.c_str());
	}
	TTF_CloseFont(font);

	//Load Stat info
	levelcoins[One_] = totalFromFile("assets/score/one.txt", "coins");
	levelcoins[Two_] = totalFromFile("assets/score/two.txt", "coins");
	levelcoins[Three_] = totalFromFile("assets/score/three.txt", "coins");
	levelcoins[Four_] = totalFromFile("assets/score/four.txt", "coins");

	// Load title music
	game->titleState.titleMusic = Mix_LoadMUS("assets/music/themesong.ogg");
	if (game->titleState.titleMusic == NULL) {
		std::cerr << "Unable to initialize titlescreen music! SDL_Error: " << Mix_GetError() << std::endl;
	}
	SoundCache_CreateSound("assets/sounds/menu.ogg", "menu");
}



void LoadHighScoreStateAssets(Game* game) {
	// Load font
	TTF_Font* font = TTF_OpenFont("assets/fonts/minnie\'shat.ttf", 30);
	if (!font) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return;
	}
	// TO-DO: Read this in from a file.
	std::string scorePath = "assets/score/score.txt";
	// Load file
	if (!FileLoader_LoadScores(scorePath.c_str(), game->highScoreState.scores)) {
		std::cerr << "Error: Unable to load scores from path " << std::endl;
		return;
	}
	//Loading coin total
	int cointotal = 0;
		for (int i = 0; i < (NumLevels_ -1); i++){
		 cointotal += levelcoins[i];
	}
	levelcoins[Total_] = cointotal;
	
 	game->highScoreState.scoreType[0]= "Hats Collected: ";
 	game->highScoreState.scoreType[1]= "Levels Won: ";
 	game->highScoreState.scoreType[2]= "Times Murdered: ";
 	game->highScoreState.scoreType[3]= "Fallen to Hell: ";
 	game->highScoreState.scoreType[4]= "Demons Killed: ";
 	game->highScoreState.scoreType[5]= "Coins Collected: ";
	
	// Create textures for the current high scores
	SDL_Color scoreColor = {255, 255, 255, 255};
	for (int highScoreIndex = 0; highScoreIndex < Constants::MaxHighScores_; highScoreIndex++) {
		std::string msg = game->highScoreState.scoreType[highScoreIndex];
		msg.append(std::to_string(game->highScoreState.scores[highScoreIndex]));
		if (highScoreIndex == 0){
			std::string totalHats = std::to_string(HatTypes_Empty -1);
			msg.append("/"+ totalHats);
		}
		if (highScoreIndex == 1){
			std::string totalLevels = std::to_string(Constants::MaximumLevels_);
			msg.append("/"+ totalLevels);
		}
		if (highScoreIndex == 5){
			std::string totalCoins = std::to_string(levelcoins[Total_]);
			msg.append("/"+ totalCoins);
		}
		std::string name = "high_score_";
		name.append(std::to_string(highScoreIndex));
		TextureCache_CreateTextureFromFont(game->renderer, font, scoreColor, msg.c_str(), name.c_str());
	}
	TTF_CloseFont(font);
}


void LoadLevelSelectAssets(Game* game) {
	game->levelSelectState.selection = 0;
	TTF_Font* font = TTF_OpenFont("assets/fonts/minnie\'shat.ttf", 36);
	if (!font) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return;
	}
	SDL_Color fontColor = {255, 255, 255, 255};
	SDL_Color baseColor = {100, 100, 100, 100};

	for (int levelIndex = 0; levelIndex <= game->playState.unlockedLevels; levelIndex++) {
		std::string name = std::to_string(levelIndex);
		name.append("_level");
		std::string lvl = "Chapter ";
		lvl.append(std::to_string(levelIndex));
		lvl.append(": ");
		lvl.append(LevelNames[levelIndex]);
		TextureCache_CreateTextureFromFont(game->renderer, font, baseColor, lvl.c_str(), name.c_str());
		name.append("_select");
		TextureCache_CreateTextureFromFont(game->renderer, font, fontColor, lvl.c_str(), name.c_str());
	}
	TTF_CloseFont(font);
}


void LoadOptionStateAssets(Game* game) {
        // Load music bar
        TextureCache_CreateTexture(game->renderer, "assets/hud/sound-bar.png", Constants::SoundBar_);
	// Load font
	TTF_Font* font = TTF_OpenFont("assets/fonts/minnie\'shat.ttf", 75);
	if (!font) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return;
	}
	//Game options
	TTF_SetFontHinting(font, TTF_HINTING_MONO);
	game->optionState.selectionStrings[0] = "Background Music";
	game->optionState.selectionStrings[1] = "Brightness";
	//game->optionState.selectionStrings[2] = "Options";
	//game->optionState.selectionStrings[3] = "Quit";

	SDL_Color baseColor = {255, 255, 255, 255};
	SDL_Color selectedColor = {100, 100, 100, 100};
	for (int selectionIndex = 0; selectionIndex < Constants::OptionScreenSelections_; selectionIndex++) {
		std::string base = game->optionState.selectionStrings[selectionIndex];
		base.append("_base");
		TextureCache_CreateTextureFromFont(game->renderer, font, baseColor, game->optionState.selectionStrings[selectionIndex], base.c_str());
		std::string select = game->optionState.selectionStrings[selectionIndex];
		select.append("_select");
		TextureCache_CreateTextureFromFont(game->renderer, font, selectedColor, game->optionState.selectionStrings[selectionIndex], select.c_str());
	}
	TTF_CloseFont(font);
}



void LoadZoneIntroAssets(Game* game, String128 name) {
	Mix_HaltChannel(-1);
	game->zoneIntroState.alpha = 0.f;
	game->zoneIntroState.elapsed = 0;
	game->zoneIntroState.startScene.current = 0;
	TTF_Font* font = TTF_OpenFont("assets/fonts/BadMofo.ttf", 50);
	if (!font) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return;
	}

	SDL_Color color = {255, 255, 255, 255};
	TextureCache_CreateTextureFromFont(game->renderer, font, color, name, Constants::ZoneName_);
	TTF_CloseFont(font);
}

//Loading the stats for the end of the level
void LoadLevelStatAssets(Game* game) {
	Mix_HaltMusic();
	// Load font
	TTF_Font* font = TTF_OpenFont("assets/fonts/minnie\'shat.ttf", 18);
	if (!font) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return;
	}
	const char * coinPath = "assets/score/coins.txt";
	std::string scorePath = "";
	switch (game->playState.levelSelection){
				case 1:
				{
					scorePath = "assets/score/one.txt";
					int levelcoins = totalFromFile(coinPath, "one");
					if (scores[Coins_] > levelcoins){
						Scores_Update(coinPath, "one", std::to_string(scores[Coins_]).c_str());
					} 
					break;
				}
				case 2:
				{
					scorePath = "assets/score/two.txt";
					int levelcoins = totalFromFile(coinPath, "two");
					if (scores[Coins_] > levelcoins){
						Scores_Update(coinPath, "two", std::to_string(scores[Coins_]).c_str());
					} 
					break;
				}
				case 3:
				{
					scorePath = "assets/score/three.txt";
					int levelcoins = totalFromFile(coinPath, "three");
					if (scores[Coins_] > levelcoins){
						Scores_Update(coinPath, "three", std::to_string(scores[Coins_]).c_str());
					} 
					break;
				}
				case 4:
				{
					scorePath = "assets/score/four.txt";
					int levelcoins = totalFromFile(coinPath, "four");
					if (scores[Coins_] > levelcoins){
						Scores_Update(coinPath, "four", std::to_string(scores[Coins_]).c_str());
					} 
					break;
				}
				default:
					break;
	}
	CoinScore_Update();
	// Load file
	if (!FileLoader_LoadLevelScores(scorePath.c_str(), numPossibleScores)) {
		std::cerr << "Error: Unable to load scores from path " << std::endl;
		return;
	}
	const char * headername = "levelheader";
	const char * header = "--------Level Stats-------- ";
	scoreType[GameTime_] = "Game Time: ";
 	scoreType[Hats_]= "Hats Collected: ";
 	scoreType[Coins_]= "Coins Collected: ";
 	scoreType[Deaths_]= "Times Murdered: ";
 	scoreType[Fallen_]= "Fallen to Hell: ";
 	scoreType[Demons_]= "Demons Killed: ";
 	scoreType[DemonBats_]= "Demon Bats Killed: ";
 	scoreType[Muffins_]= "Demon Muffins Killed: ";
 	scoreType[Oranges_]= "Demon Oranges Killed: ";
 	scoreType[Apples_]= "Apples Killed: ";
	
	// Create textures for the current high scores
	SDL_Color scoreColor = {255, 255, 255, 255};
	if (game->gameState == GameState_Win){
		Sound_Play(SoundCache_GetSound("win"), 0);
		scoreColor = {0, 0, 0, 0};
	} else if (game->gameState == GameState_Lose){
		Sound_Play(SoundCache_GetSound("death"), 0);
	  scoreColor = {255, 255, 255, 255};
	}
	TextureCache_CreateTextureFromFont(game->renderer, font, scoreColor, header, headername);
	for (int highScoreIndex = 0; highScoreIndex < NumScoreTypes_; highScoreIndex++) {
		std::string msg = scoreType[highScoreIndex];
		if (highScoreIndex == GameTime_){
			int seconds = ((int)(scores[highScoreIndex] / Constants::Second_)) % 60;
			int minutes = ((int)(scores[highScoreIndex] / Constants::Second_)) / 60;
			std::string secStr = std::to_string(seconds);
			std::string minStr = std::to_string(minutes);
			if (seconds < 10) {
			  secStr = "0" + secStr;
			}
			if (minutes < 10) {
			  minStr = "0" + minStr;
			}
			std::string scoreStr = minStr + ":" + secStr;
			msg.append(scoreStr);
		} else {
			msg.append(std::to_string(scores[highScoreIndex]));	
		}
		if (highScoreIndex != Deaths_ && highScoreIndex != Fallen_ && highScoreIndex != GameTime_){
			std::string totalNum = std::to_string(numPossibleScores[highScoreIndex]);
			msg.append("/"+ totalNum);
		}
		if (numPossibleScores[highScoreIndex] != 0 || (highScoreIndex == Deaths_ 
			|| highScoreIndex == Fallen_ || highScoreIndex == GameTime_)){
			numDisplay++;
		}
		std::string name = "level_score_";
		name.append(std::to_string(highScoreIndex));
		TextureCache_CreateTextureFromFont(game->renderer, font, scoreColor, msg.c_str(), name.c_str());
	}
	TTF_CloseFont(font);
}

//Loading the stats for the end of the level
void LoadCredits(Game* game) {
	// Load font
	TTF_Font* font = TTF_OpenFont("assets/fonts/Gotham-Bold.otf", 18);
	if (!font) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return;
	}

	const char * creditsname = "credits";
	const char * credits = "Created By: HaMaMiGa";
	const char * creditsname1 = "harry";
	const char * credits1 = "Harry Cohen, Matt Kosarek, ";
	const char * creditsname2 = "matt";
	const char * credits2 = "Mindy Seto, Gavi Rawson";
	SDL_Color scoreColor = {255, 255, 255, 255};
	TextureCache_CreateTextureFromFont(game->renderer, font, scoreColor, credits, creditsname);
	TextureCache_CreateTextureFromFont(game->renderer, font, scoreColor, credits1, creditsname1);
	TextureCache_CreateTextureFromFont(game->renderer, font, scoreColor, credits2, creditsname2);
	TTF_CloseFont(font);
}

bool LoadPlayStateAssets(Game* game, int chapter) {
	Mix_HaltChannel(-1);
	//Resetting level scores
	LevelScore_Reset(&game->playState.restarted);
	//If last level load credits
	if (chapter == Credits_){
		LoadCredits(game);
	}
	// Initialize caches
	TextureCache* tcache = TextureCache_GetCache();
	tcache->levelIndex = tcache->index;
	SoundCache* scache = SoundCache_GetCache();
	scache->levelIndex = scache->index;
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
	TextureCache_CreateTexture(game->renderer, "assets/hud/health-bar.png", Constants::HealthBar_);
	TextureCache_CreateTexture(game->renderer, "assets/hud/bullet-hud.png", Constants::BulletHud_);
	TextureCache_CreateTexture(game->renderer, "assets/hud/knife-hud.png", Constants::KnifeHud_);
	TextureCache_CreateTexture(game->renderer, "assets/hud/time-bar.png", Constants::TimeBar_);
	TextureCache_CreateTexture(game->renderer, "assets/hud/coin-bar.png", Constants::CoinBar_);
	SDL_SetTextureBlendMode(TextureCache_CreateTexture(game->renderer, "assets/hats/glamour-hat-glow.png", Constants::GlamourGlow_)->sdltexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(TextureCache_CreateTexture(game->renderer, "assets/hats/hat-glow.png", Constants::HatGlow_)->sdltexture, SDL_BLENDMODE_BLEND);
	
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
	game->playState.hudFont = TTF_OpenFont("assets/fonts/Gotham-Bold.otf", 30);
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
	SoundCache_CreateSound("assets/sounds/heli.ogg", "heli");
	SoundCache_CreateSound("assets/sounds/beer.ogg", "beer");
	SoundCache_CreateSound("assets/sounds/ow.ogg", "ow");
	SoundCache_CreateSound("assets/sounds/thud.ogg", "thud");
	SoundCache_CreateSound("assets/sounds/coin.ogg", "coin");
	SoundCache_CreateSound("assets/sounds/chef.ogg", "chef");
	SoundCache_CreateSound("assets/sounds/knife.ogg", "knife");
	SoundCache_CreateSound("assets/sounds/pew.ogg", "pew"); 
	SoundCache_CreateSound("assets/sounds/circus.ogg", "circus");
	SoundCache_CreateSound("assets/sounds/circusjump.ogg", "circusjump");
	SoundCache_CreateSound("assets/sounds/win.ogg", "win"); 
	SoundCache_CreateSound("assets/sounds/death.ogg", "death");  
	TextureCache_CreateTexture(game->renderer, "assets/hats/bullet.png", "bullet");
	TextureCache_CreateTexture(game->renderer, "assets/hats/knife.png", "knife");
	SDL_Color color = { game->playState.cBag.interactableComponent->r, game->playState.cBag.interactableComponent->g, game->playState.cBag.interactableComponent->b, 1 };
	TextureCache_CreateTextureFromFont(game->renderer, game->playState.cBag.interactableComponent->hoverFont, color, "'X' to Pick Up", "bullet-pickup");
	SDL_SetTextureBlendMode(TextureCache_CreateTexture(game->renderer, "assets/hats/miner-shader.png", "miner-shader")->sdltexture, SDL_BLENDMODE_MOD);
	TTF_SetFontHinting(game->playState.scoreFont, TTF_HINTING_MONO);
	TTF_SetFontHinting(game->playState.healthFont, TTF_HINTING_MONO);

	// Initialize systems
	AISystem_Initialize(&game->playState.aiSystem, &game->playState.cBag);
	CameraSystem_Initialize(&game->playState.cameraSystem, &game->playState.cBag, &game->playState.chapter);
	InputSystem_Initialize(&game->playState.inputSystem, &game->playState.cBag);
	MovementSystem_Initialize(&game->playState.movementSystem, &game->playState.cBag);
	PhysicsSystem_Initialize(&game->playState.physicsSystem, &game->playState.cBag,  &game->playState.chapter.tileMap, &game->playState.chapter);
	RenderSystem_Initialize(&game->playState.renderSystem, &game->playState.cBag, &game->playState.chapter.tileMap, game->playState.hudFont);
	GoalSystem_Initialize(&game->playState.goalSystem, &game->playState.cBag, &game->playState.chapter);
	SoundSystem_Initialize(&game->playState.soundSystem, &game->playState.cBag, game->playState.chapter.music);
	KillSystem_Initialize(&game->playState.killSystem, &game->playState.cBag);
	InteractionSystem_Initialize(&game->playState.interactionSystem, &game->playState.cBag, game);

	// Pause state
	TextureCache_CreateTexture(game->renderer, "assets/interactables/speech-bubble.png", "speech-bubble");

	// Load score fonts
	SDL_Color scoreColor = {255, 255, 255, 1};
	for (int i = 0; i < 10; i++) {
		TextureCache_CreateTextureFromFont(game->renderer, game->playState.renderSystem.defaultFont, scoreColor, std::to_string(i).c_str(), std::to_string(i).c_str());
	}
	TextureCache_CreateTextureFromFont(game->renderer, game->playState.renderSystem.defaultFont, scoreColor, ":", ":");
	TextureCache_CreateTextureFromFont(game->renderer, game->playState.renderSystem.defaultFont, scoreColor, "/", "/");
	// Set loaded variable
	game->playState.loaded = true;

	return true;
}


void FreePlay(Game* game) {
	// Free caches
	EntityCache_Free();
	ComponentBag_Free(&game->playState.cBag);
	TextureCache_FreeLevel();
	//SoundCache_FreeLevel();
	Mix_HaltChannel(Constants::DiscoChannel_);

	// Free cutscenes
	strcpy(game->playState.chapter.name, "");
	game->zoneIntroState.startScene.slideCount = 0;
	game->zoneIntroState.startScene.current = 0;
	game->zoneIntroState.endScene.slideCount = 0;
	game->zoneIntroState.endScene.current = 0;

	// Free fonts
	TTF_CloseFont(game->playState.hudFont);
	TTF_CloseFont(game->playState.scoreFont);
	TTF_CloseFont(game->playState.healthFont);

	// Delete all pointers in ai system
	AISystem_Free(&game->playState.aiSystem);
	CameraSystem_Free(&game->playState.cameraSystem);
	InputSystem_Free(&game->playState.inputSystem);
	MovementSystem_Free(&game->playState.movementSystem);
	PhysicsSystem_Free(&game->playState.physicsSystem);
	RenderSystem_Free(&game->playState.renderSystem);
	GoalSystem_Free(&game->playState.goalSystem);
	//	SoundSystem_Free(&game->playState.soundSystem);
	KillSystem_Free(&game->playState.killSystem);
	InteractionSystem_Free(&game->playState.interactionSystem);

	// Set loaded variable.
	game->playState.loaded = false;
}




