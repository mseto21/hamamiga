#include "Game.h"
#include "TextureCache.h"
#include "EntityCache.h"
#include "SoundCache.h"
#include "StateLoader.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using namespace std;
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

	// Initialize entity cache
	if (EntityCache_GetCache() == NULL) {
		std::cerr << "Error: The entity cache failed to initialize!" << std::endl;
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
	game->playState.cBag.freed = true;
	game->playState.loaded = false;
	game->playState.currentLevel = 1;

	// Enter title state
	game->gameState = GameState_Intro;
	game->running = true;
	return true;
}


void RenderIntro(Game* game, uint32 elapsed) {
	// Check if the intro is done
	game->introState.elapsed += elapsed;
	game->introState.alpha = 1 - (((float)game->introState.elapsed) / ((float)Constants::IntroTime_));
	if ( game->introState.elapsed  >= Constants::IntroTime_) {
		game->gameState = GameState_Title;
	}

	// Render intro
	Texture* fader = TextureCache_GetTexture(Constants::TitleFader_);
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	Texture* menuOverlay = TextureCache_GetTexture(Constants::MenuOverlay_);

	if (background) {
	  RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	}
	if (fader) { 
		RenderSystem_Render_xywh(game->renderer, 0, 0, menuOverlay->w, menuOverlay->h, NULL,menuOverlay);
		SDL_SetTextureAlphaMod(fader->sdltexture, (game->introState.alpha * 255));
		RenderSystem_Render_xywh(game->renderer, 0, 0, fader->w, fader->h, NULL,  fader);
	}
	SDL_RenderPresent(game->renderer);
}


void UpdateTitle(Game* game, bool* keysdown, bool* keysup) {
	// Update their options
	if (keysdown[SDLK_w] && !game->titleState.w) {
		if (!game->titleState.w) {
			game->titleState.w = true;
			game->titleState.s = false;
			game->titleState.selection--;
			game->titleState.selection %= Constants::TitleScreenSelections_;
		}
	}
	if (keysup[SDLK_w]) {
		game->titleState.w = false;
	}

	if (keysdown[SDLK_s] && !game->titleState.s) {
		game->titleState.w = false;
		game->titleState.s = true;
		game->titleState.selection++;
		game->titleState.selection %= Constants::TitleScreenSelections_;
	}
	if (keysup[SDLK_s]) {
		game->titleState.s = false;
	}

	// Check for music playing
	if (Mix_PlayingMusic() == 0) {
		Mix_PlayMusic(game->titleState.titleMusic, 0);
	}

	// Set the next state
	if (keysdown[SDLK_RETURN]) {
		switch (game->titleState.selection) {
			case 0:
				Mix_HaltMusic();
				game->playState.loaded = LoadPlayStateAssets(game, game->playState.currentLevel);
				if (!game->playState.loaded) {
					std::cerr << "Error: Unable to find game with level " << game->playState.currentLevel << std::endl;
					game->gameState = GameState_Title;
					break;
				} else {
					game->gameState = GameState_Play;
				}
				LoadZoneIntroAssets(game, game->playState.chapter.name);
				game->gameState = GameState_ZoneIntro;
				break;
			case 1:
				game->gameState = GameState_HighScore;
				LoadHighScoreStateAssets(game);
				break;
			case 2:
				// Change to option game state
				break;
			case 3:
				Mix_HaltMusic();
				game->gameState = GameState_Closing;
				break;
		}
	}
}


void RenderTitle(Game* game) {
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	for (int selectionIndex = 0; selectionIndex < Constants::TitleScreenSelections_; selectionIndex++) {
		Texture* selection;
		if (selectionIndex == game->titleState.selection) {
			std::string base = game->titleState.selectionStrings[selectionIndex];
			base.append("_base");
			selection = TextureCache_GetTexture(base.c_str());
		} else {
			std::string select = game->titleState.selectionStrings[selectionIndex];
			select.append("_select");
			selection = TextureCache_GetTexture(select.c_str());
		}
		int renderX = Constants::ScreenWidth_ / 2 - selection->w / 2;
		int renderY = selectionIndex * (Constants::ScreenHeight_ / Constants::TitleScreenSelections_);
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, selection->w, selection->h, NULL, selection);
	}
	SDL_RenderPresent(game->renderer);
}


void UpdateHighScore(Game* game, bool* keysdown) {
	(void) keysdown;
	(void) game;
}


void RenderHighScore(Game* game, uint32 elapsed) {
	(void) elapsed;
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	SDL_RenderClear(game->renderer);
	if (background) RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	for (int highScoreIndex = 0; highScoreIndex < Constants::MaxHighScores_; highScoreIndex++) {
		std::string name = "high_score_";
		name.append(std::to_string(game->highScoreState.scores[highScoreIndex]));
		Texture* score = TextureCache_GetTexture(name.c_str());
		int renderX = Constants::ScreenWidth_ / 2 - score->w / 2;
		int renderY = highScoreIndex * (Constants::ScreenHeight_ / Constants::MaxHighScores_);
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, score->w, score->h, NULL, score);
	}
	SDL_RenderPresent(game->renderer);
}


void UpdatePause(Game* game, uint32 elapsed) {
	// TO-DO: Implement some sort of pause
	(void) elapsed;
	(void) game;
}


void RenderLose(Game* game) {
	Texture* background = TextureCache_GetTexture(Constants::LoseBackground_);
	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	SDL_RenderPresent(game->renderer);
}


void RenderWin(Game* game) {
	Texture* background = TextureCache_GetTexture(Constants::WinBackground_);
	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	SDL_RenderPresent(game->renderer);
}


void RenderZoneIntro(Game* game, uint32 elapsed) {
	game->zoneIntroState.elapsed += elapsed;

	if (game->zoneIntroState.elapsed >= (game->playState.chapter.startScene.slideCount * Constants::CutSceneSlideTime_) + Constants::ZoneIntroTime_) {
		game->gameState = GameState_Play;
	}
	// Render fade
	if (game->zoneIntroState.elapsed >= game->playState.chapter.startScene.slideCount * Constants::CutSceneSlideTime_) {
		CameraSystem_Update(&game->playState.cameraSystem);
		SDL_RenderClear(game->renderer);
		game->zoneIntroState.alpha = 1 - (((float)game->zoneIntroState.elapsed - (game->playState.chapter.startScene.slideCount * Constants::CutSceneSlideTime_)) / ((float)Constants::ZoneIntroTime_));
		
		// Render intro
		Texture* fader = TextureCache_GetTexture(Constants::TitleFader_);
		SDL_RenderClear(game->renderer);
		if (fader) { 
			RenderSystem_Update(&game->playState.renderSystem, game->renderer, elapsed);
			Texture* name = TextureCache_GetTexture("zone_name");
			if (!name) {
				std::cerr << "Error: Unable to load the zone's name texture" << std::endl;
				return;
			}
			int renderX = Constants::ScreenWidth_ / 2 - name->w / 2;
			int renderY = Constants::ScreenHeight_/2 - name->h / 2;
			RenderSystem_Render_xywh(game->renderer, renderX, renderY, name->w, name->h, NULL, name);
			SDL_SetTextureAlphaMod(fader->sdltexture, (game->zoneIntroState.alpha * 255));
			RenderSystem_Render_xywh(game->renderer, 0, 0, fader->w, fader->h, NULL,  fader);
		}
		SDL_RenderPresent(game->renderer);
	} else { // Render cut scene
		SDL_RenderClear(game->renderer);
		Texture* scene = game->playState.chapter.startScene.slides[game->playState.chapter.startScene.current];
		if (scene) {
			RenderSystem_Render_xywh(game->renderer, 0, 0, scene->w, scene->h, NULL, scene);
			SDL_RenderPresent(game->renderer);
		}
		if (game->zoneIntroState.elapsed >= Constants::CutSceneSlideTime_ * (game->playState.chapter.startScene.current + 1)) { // Go to next slide
			game->playState.chapter.startScene.current++;
		}
	}
}


void UpdatePlay(Game* game) {
	AISystem_Update(&game->playState.aiSystem);
	MovementSystem_Update(&game->playState.movementSystem);
	PhysicsSystem_Update(&game->playState.physicsSystem);
	KillSystem_Update(&game->playState.killSystem);
	switch (GoalSystem_Update(&game->playState.goalSystem, Constants::OptimalTime_)) {
		case GameResult_Fell:
			// Trigger some different animation, noise, etc.
		case GameResult_Killed:
			game->gameState = GameState_Lose;
			break;
		case GameResult_Won:
			game->playState.currentLevel++;
			game->gameState = GameState_Win;
			Mix_VolumeMusic(MIX_MAX_VOLUME/4);
			Sound_Play(SoundCache_GetSound("nj"), 0);
			game->gameState = GameState_Win;
		default:
			break;
	}
}


void RenderPlay(Game* game, Uint32 elapsed) {
	CameraSystem_Update(&game->playState.cameraSystem);
	SoundSystem_Update(&game->playState.soundSystem, elapsed);
	SDL_RenderClear(game->renderer);
	RenderSystem_Update(&game->playState.renderSystem, game->renderer, elapsed);
	SDL_RenderPresent(game->renderer);
}


void FreePlay(Game* game) {
	if (game->playState.loaded) {
		if (!game->playState.cBag.freed) {
			ComponentBag_Free(&game->playState.cBag);
		}
		game->playState.loaded = false;
		Mix_FreeMusic(game->playState.chapter.music);
		Mix_HaltChannel(Constants::DiscoChannel_);
	}
	EntityCache_RemoveAll();
}


void ReturnFromPlay(Game* game) {
	FreePlay(game);
	game->gameState = GameState_Title;
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
	memset(&keysdown, 0, sizeof(keysdown));
	bool keysup[Constants::NumKeys_];
	memset(&keysup, 0, sizeof(keysup));

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
							if (game->gameState == GameState_Play || game->gameState == GameState_Lose || game->gameState == GameState_Win){
								game->gameState = GameState_Returning;
								Mix_HaltChannel(2);
								Mix_FreeMusic(game->playState.chapter.music);
							}
							else{
								game->gameState = GameState_Title;
							}
							break;
						case SDLK_p:
							if (game->gameState != GameState_Title && game->gameState != GameState_HighScore &&
								game->gameState != GameState_Intro){
								FreePlay(game);
								if (!LoadPlayStateAssets(game, game->playState.currentLevel)) {
									std::cerr << "Error: Unable to find game with level " << game->playState.currentLevel << std::endl;
									game->gameState = GameState_Title;
								} else {
									game->gameState = GameState_Play;
								}
							}
							break;
						case SDLK_u:
							if (game->gameState == GameState_Play) {
								game->gameState = GameState_Win;
								game->playState.currentLevel++;
							}
							break;
						default:
							keysdown[event.key.keysym.sym % Constants::NumKeys_] = true;
							keysup[event.key.keysym.sym % Constants::NumKeys_] = false;
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
		if (game->gameState == GameState_Play) {
				InputSystem_Update(&game->playState.inputSystem, keysdown, keysup);
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
				case GameState_Pause:
					UpdatePause(game, elapsed);
					break;
				default:
					break;
			}
			lag -= Constants::OptimalTime_;
		}

		// Render game state
		switch(game->gameState) {
			case GameState_Intro:
				RenderIntro(game, elapsed);
				break;
			case GameState_Title:
				RenderTitle(game);
				break;
			case GameState_ZoneIntro:
				RenderZoneIntro(game, elapsed);
				break;
			case GameState_Play:
				RenderPlay(game, elapsed);
				break;
			case GameState_HighScore:
				RenderHighScore(game, elapsed);
				break;
			case GameState_Pause:
				break;
			case GameState_Win:
				RenderWin(game);
				break;
	  		case GameState_Lose:
	  			RenderLose(game);
				break;
			case GameState_Returning:
				ReturnFromPlay(game);
				break;
			case GameState_Closing:
				game->running = false;
				Game_Close(game);
				break;
			default:
				break;
		}
	}
}


void Game_Close(Game* game) {
	FreePlay(game);
	TTF_CloseFont(game->playState.scoreFont);
	TTF_CloseFont(game->playState.healthFont);
	Mix_FreeMusic(game->titleState.titleMusic);
	TextureCache_Free();
	EntityCache_Free();
	SoundCache_Free();
	Mix_CloseAudio();
	TTF_Quit();
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}
