#include "Game.h"
#include "TextureCache.h"
#include "SoundCache.h"
#include "StateLoader.h"
#include "UIRenderSystem.h"
#include "StatSystem.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using namespace std;
const char * lval = "1";

bool Game_Initialize(Game* game) {
	// Initialize sdl
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		return false;
	}

	// Initialize game window
	game->window = SDL_CreateWindow("Fat Hactory", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		Constants::ScreenWidth_, Constants::ScreenHeight_, SDL_WINDOW_SHOWN);
	if (!game->window ) {
		std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Initialize renderer
	game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
	if (!game->renderer) {
		std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Initialize image lib
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) && imgFlags)) {
		std::cerr << "Image library could not initialize! SDL Error: " << IMG_GetError() << std::endl;
		return false;
	}

	// Initialize font lib
	if (TTF_Init() == -1) {
		std::cerr << "TTF library could not initialize! SDL Error: " << TTF_GetError() << std::endl;
		return false;
	}

	// Initialize mixer
	if(Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1) {
    	std::cerr << "Mixer library could not initialize! SDL Error: " << Mix_GetError() << std::endl;
        return false;    
    }

	// Initialize texture cache
	if (TextureCache_GetCache() == NULL) {
		std::cerr << "Error: The texture cache failed to initialize!" << std::endl;
		return false;
	}

	// Initialize sound cache
	if (SoundCache_GetCache() == NULL) {
		std::cerr << "Error: The sound cache failed to initialize!" << std::endl;
		return false;
	}

	// Initialize states
	LoadIntroStateAssets(game);
	LoadTitleStateAssets(game);
	memset(&game->highScoreState.scores, 0, sizeof(game->highScoreState.scores));
	game->playState.loaded = false;

	//Initialize levels
	const char* levelpath = "assets/score/levels.txt";
	int totallevels = totalFromFile(levelpath, "total");
	game->playState.unlockedLevels = (totallevels+1);
	game->playState.levelSelection = 1;

	// Enter title state
	game->gameState = GameState_Intro;
	game->running = true;
	return true;
}


void UpdateTitle(Game* game, bool* keysdown, bool* keysup) {
    if (keysdown[SDLK_UP % Constants::NumKeys_] && keysup[SDLK_UP % Constants::NumKeys_]) {
		game->titleState.selection--;
		game->titleState.selection %= Constants::TitleScreenSelections_;
		Sound_Play(SoundCache_GetSound("menu"), 0);
		keysup[SDLK_UP % Constants::NumKeys_] = false;
	} else if (keysdown[SDLK_DOWN % Constants::NumKeys_] && keysup[SDLK_DOWN % Constants::NumKeys_]) {
		game->titleState.selection++;
		game->titleState.selection %= Constants::TitleScreenSelections_;
		Sound_Play(SoundCache_GetSound("menu"), 0);
		keysup[SDLK_DOWN % Constants::NumKeys_] = false;
	}

	// Check for music playing
	if (Mix_PlayingMusic() == 0) {
		Mix_PlayMusic(game->titleState.titleMusic, 0);
	}

	// Set the next state
	if (keysdown[SDLK_RETURN]) {
		keysup[SDLK_RETURN % Constants::NumKeys_] = false;
		switch (game->titleState.selection) {
			case 0:
				game->gameState = GameState_LevelSelect;
				LoadLevelSelectAssets(game);
				break;
			case 1:
				game->playState.levelSelection = 0;
				game->gameState = GameState_LoadPlay;
				break;
			case 2:
				game->gameState = GameState_Options;
				LoadOptionStateAssets(game);
				break;
			case 3:
				game->gameState = GameState_HighScore;
				LoadHighScoreStateAssets(game);
				break;
			case 4:
				Mix_HaltMusic();
				game->gameState = GameState_Closing;
				break;
		}
	}
}


void UpdateHighScore(Game* game, bool* keysdown) {
	(void) keysdown;
	(void) game;
}


void UpdateOptions(Game* game, bool* keysdown, bool* keysup) {
	// Update their options
	if (keysdown[SDLK_UP % Constants::NumKeys_] && keysup[SDLK_UP % Constants::NumKeys_]) {
		game->optionState.selection--;
		game->optionState.selection %= Constants::OptionScreenSelections_;
		keysup[SDLK_UP % Constants::NumKeys_] = false;
	}

	if (keysdown[SDLK_DOWN % Constants::NumKeys_] && keysup[SDLK_DOWN % Constants::NumKeys_]) {
		game->optionState.selection++;
		game->optionState.selection %= Constants::OptionScreenSelections_;
		keysup[SDLK_DOWN % Constants::NumKeys_] = false;
	}

	// Set the next state
	switch (game->optionState.selection) {
		case 0:
			if (keysdown[SDLK_RIGHT % Constants::NumKeys_]){
				if (game->optionState.musicVolume < Constants::MaxVolume_) {
					game->optionState.musicVolume += Constants::VolumeUnit_;
				}
			} else if (keysdown[SDLK_LEFT % Constants::NumKeys_]){
				if (game->optionState.musicVolume > 0){
					game->optionState.musicVolume -= Constants::VolumeUnit_;
				}
			}
			Mix_VolumeMusic(game->optionState.musicVolume);
			break;
			/*case 1:
			if (keysdown[SDLK_d]){
				if (game->optionState.windowBrightness < Constants::MaxBrightness_) {
					game->optionState.windowBrightness += Constants::BrightnessUnit_;
				}
			} else if (keysdown[SDLK_a]){
				game->optionState.windowBrightness -= Constants::BrightnessUnit_;
				if (game->optionState.windowBrightness < 0){
					game->optionState.windowBrightness = 0;
				}
			}
			SDL_SetWindowBrightness(game->window, game->optionState.windowBrightness);
			SDL_UpdateWindowSurface(game->window);*/
			break;
	}
}

void UpdateLevelSelect(Game* game, bool* keysdown, bool* keysup) {
	if (keysdown[SDLK_UP % Constants::NumKeys_] && keysup[SDLK_UP % Constants::NumKeys_]) {
		if (game->levelSelectState.selection == 0)
			game->levelSelectState.selection = game->playState.unlockedLevels + 1;
		game->levelSelectState.selection--;
		game->levelSelectState.selection %= (game->playState.unlockedLevels + 1);
		keysup[SDLK_UP % Constants::NumKeys_] = false;
	}

	if (keysdown[SDLK_DOWN % Constants::NumKeys_] && keysup[SDLK_DOWN % Constants::NumKeys_]) {
		game->levelSelectState.selection++;
		game->levelSelectState.selection %= (game->playState.unlockedLevels + 1);
		keysup[SDLK_DOWN % Constants::NumKeys_] = false;
	}

	if (keysdown[SDLK_RETURN % Constants::NumKeys_] && keysup[SDLK_RETURN % Constants::NumKeys_]) {
		game->playState.levelSelection = game->levelSelectState.selection;
		std::cout << game->playState.levelSelection << std::endl;
		game->gameState = GameState_LoadPlay;
		keysup[SDLK_RETURN % Constants::NumKeys_] = false;
	}
}

void UpdatePause(Game* game, bool* keysdown, bool* keysup) {
	if (keysdown[SDLK_SPACE] && keysup[SDLK_SPACE]) {
		game->pauseState.pauseIndex++;
		keysup[SDLK_SPACE] = false;
		keysdown[SDLK_SPACE] = false;
	}
}

void UpdatePlay(Game* game) {
	AISystem_Update(&game->playState.aiSystem);
	MovementSystem_Update(&game->playState.movementSystem);
	PhysicsSystem_Update(&game->playState.physicsSystem);
	InteractionSystem_Update(&game->playState.interactionSystem);
	KillSystem_Update(&game->playState.killSystem);
	switch (GoalSystem_Update(&game->playState.goalSystem, Constants::OptimalTime_)) {
		case GameResult_Fell:
			// Trigger some different animation, noise, etc.
		case GameResult_Killed:
			game->gameState = GameState_Lose;
			LoadLevelStatAssets(game);
			break;
		case GameResult_Won:
			switch (game->playState.levelSelection){
				case 1:
					Scores_Update("assets/score/levels.txt", "one", lval);
					break;
				case 2:
					Scores_Update("assets/score/levels.txt", "two", lval);
					break;
				case 3:
					Scores_Update("assets/score/levels.txt", "three", lval);
					break;
				case 4:
				        Scores_Update("assets/score/levels.txt", "four", lval);
					break;
				default:
					break;
			}
			if (game->playState.levelSelection == game->playState.unlockedLevels)
				game->playState.unlockedLevels++;
			game->gameState = GameState_Win;
			LoadLevelStatAssets(game);
		default:
			break;
	}
}


void RenderPlay(Game* game, Uint32 elapsed) {
	CameraSystem_Update(&game->playState.cameraSystem);
	SoundSystem_Update(&game->playState.soundSystem, elapsed);
	SDL_RenderClear(game->renderer);
	RenderSystem_Update(&game->playState.renderSystem, game->renderer, elapsed, game->playState.levelSelection);
	SDL_RenderPresent(game->renderer);
}


void Game_RunLoop(Game* game) {
	if (!game) {
		std::cerr << "The game instance was not initialized!" << std::endl;
		return;
	}

	// Time Management variables
	SDL_Event event;
	Uint32 currentTime = SDL_GetTicks();
	Uint32 lastTime = 0;
	Uint32 elapsed;
	Uint32 lag = 0;

	// Input variables
	bool keysdown[Constants::NumKeys_];
	memset(&keysdown, false, Constants::NumKeys_);
	bool keysup[Constants::NumKeys_];
	memset(&keysup, true, Constants::NumKeys_);

	while (game->running) {
		currentTime = SDL_GetTicks();
		elapsed = currentTime - lastTime;
		lastTime = currentTime;
		lag += elapsed;

		// Poll for input
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
				case SDL_QUIT:
					game->gameState = GameState_Closing;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							game->gameState = GameState_Closing;
							break;
						case SDLK_m:
							if (game->gameState == GameState_Win || game->gameState == GameState_Lose 
								|| game->gameState == GameState_Play || game->gameState == GameState_ZoneIntro
								|| game->gameState == GameState_Pause) {
								Mix_HaltMusic();
								Mix_HaltChannel(-1);
								game->gameState = GameState_Returning;
							} else {
								game->gameState = GameState_Title;
							}
							break;
					        case SDLK_r:
						        if (game->gameState == GameState_Win || game->gameState == GameState_Lose
							    || game->gameState == GameState_Play

							    || game->gameState == GameState_Pause) {
						                 FreePlay(game);
						               	 game->playState.restarted = true;
						                 game->gameState = GameState_LoadPlay;
						  }
						case SDLK_n:
						if (game->gameState == GameState_Win){
							if (game->gameState == GameState_Win || game->gameState == GameState_Lose 
								|| game->gameState == GameState_Play || game->gameState == GameState_ZoneIntro) {
								if (game->playState.levelSelection == game->playState.unlockedLevels)
									game->playState.unlockedLevels++;
								if (game->playState.levelSelection == game->playState.unlockedLevels)
									game->playState.unlockedLevels++;
								game->playState.levelSelection++;//= game->playState.unlockedLevels;
								FreePlay(game);
								game->gameState = GameState_LoadPlay;
							} 
						}
							break;
						case SDLK_p:
							if (game->gameState == GameState_ZoneIntro)
								game->gameState = GameState_Play;
							break;
						case SDLK_u:
							if (game->playState.levelSelection != 0 && game->playState.levelSelection != 5){
							if (game->gameState == GameState_Play) {
								game->gameState = GameState_Win;
								LoadLevelStatAssets(game);
								switch (game->playState.levelSelection){
									case 1:
										Scores_Update("assets/score/levels.txt", "one", lval);
										break;
									case 2:
										Scores_Update("assets/score/levels.txt", "two", lval);
										break;
									case 3:
										Scores_Update("assets/score/levels.txt", "three", lval);
										break;
									case 4:
									  Scores_Update("assets/score/levels.txt", "four", lval);
										break;
									default:
										break;
								}
								if (game->playState.levelSelection == game->playState.unlockedLevels)
									game->playState.unlockedLevels++;
							}
							break;
						}
						case SDLK_1:
						        game->playState.unlockedLevels = Constants::MaximumLevels_ + 1;
							if (game->gameState == GameState_LevelSelect)
								LoadLevelSelectAssets(game);
							break;
						default:
							keysdown[event.key.keysym.sym % Constants::NumKeys_] = true;
							break;
					}
					break;
				case SDL_KEYUP:
					keysdown[event.key.keysym.sym % Constants::NumKeys_] = false;
					keysup[event.key.keysym.sym % Constants::NumKeys_] = true;
					break;
				default:
					break;
			}
		}

		// TO-DO: I'm sad about this.
		switch (game->gameState) {
			case GameState_Play:
				InputSystem_Update(&game->playState.inputSystem, keysdown, keysup);
				break;
			case GameState_Options:
				UpdateOptions(game, keysdown, keysup);
				break;
			case GameState_Pause:
				UpdatePause(game, keysdown, keysup);
				break;
			default:
				break;
		}

		// Update game state
		while (lag > Constants::OptimalTime_) {
			switch(game->gameState) {
				case GameState_Title:
					UpdateTitle(game, keysdown, keysup);
					break;
				case GameState_Play:
					UpdatePlay(game);
					break;
				case GameState_HighScore:
					UpdateHighScore(game, keysdown);
					break;
				case GameState_LevelSelect:
					UpdateLevelSelect(game, keysdown, keysup);
					break;
				default:
					lag = Constants::OptimalTime_;
					break;
			}
			lag -= Constants::OptimalTime_;
		}

		// Once per update
		switch(game->gameState) {
			case GameState_LoadPlay:
				Mix_HaltMusic();
				LoadPlayStateAssets(game, game->playState.levelSelection);
				if (!game->playState.loaded) {
					std::cerr << "Error: Unable to find game with level " << game->playState.levelSelection << std::endl;
					game->gameState = GameState_Title;
				} else {
					LoadZoneIntroAssets(game, game->playState.chapter.name);
					game->gameState = GameState_ZoneIntro;
				}
				break;
			case GameState_Play:
				RenderPlay(game, elapsed);
				break;
			case GameState_Returning:
				FreePlay(game);
				LoadLevelSelectAssets(game);
				game->gameState = GameState_Title;
				break;
			case GameState_ReturnAndEnterLevel:
				FreePlay(game);
				game->gameState = GameState_LoadPlay;
				break;
			case GameState_Closing:
				if (game->gameState == GameState_Win || game->gameState == GameState_Lose || game->gameState == GameState_Play)
					FreePlay(game);
				game->running = false;
				Game_Close(game);
				break;
			default:
				UIRenderSystem_Render(game->gameState, game, elapsed, keysdown, keysup);
				break;
		}
	}
}


void Game_TriggerPause(Game* game, Texture** pauseTextures) {
	game->gameState = GameState_Pause;
	game->pauseState.pauseTextures = pauseTextures;
	game->pauseState.pauseIndex = 0;
}


void Game_Close(Game* game) {
	Mix_FreeMusic(game->titleState.titleMusic);
	TextureCache_Free();
	SoundCache_Free();
	Mix_CloseAudio();
	TTF_Quit();
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}
