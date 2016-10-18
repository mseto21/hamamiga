#include "Game.h"
#include "TextureCache.h"
#include "EntityCache.h"
#include "FileLoader.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using std::cout;
using std::endl;

//--------------------------------------------------------------------
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


//--------------------------------------------------------------------
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


//--------------------------------------------------------------------
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


//--------------------------------------------------------------------
bool LoadPlayStateAssets(Game* game) {
	game->playState.scoreFont = TTF_OpenFont("assets/minnie\'shat.ttf", 30);
	game->playState.healthFont = TTF_OpenFont("assets/minnie\'shat.ttf", 30);
	TextureCache_CreateTexture(game->renderer, "assets/bunny-hat.png", Constants::BunnyHat_);
	if (!game->playState.scoreFont) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return false;
	}
	if (!game->playState.healthFont) {
	    std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
	    return false;
	}
	
	TTF_SetFontHinting(game->playState.scoreFont, TTF_HINTING_MONO);
	TTF_SetFontHinting(game->playState.healthFont, TTF_HINTING_MONO);
	ComponentBag_Malloc(&game->playState.cBag);

	// TO-DO: Hardcoded fro now, but its coolio!
	FileLoader_Load(&game->playState.chapter, "assets/chapter_1/chapter_1.txt", &game->playState.cBag, game->renderer); // Hardcoded for now, but easily an array.
	
	AISystem_Initialize(&game->playState.aiSystem, &game->playState.cBag);
	CameraSystem_Initialize(&game->playState.cameraSystem, &game->playState.cBag);
	InputSystem_Initialize(&game->playState.inputSystem, &game->playState.cBag);
	MovementSystem_Initialize(&game->playState.movementSystem, &game->playState.cBag);
	PhysicsSystem_Initialize(&game->playState.physicsSystem, &game->playState.cBag,  &game->playState.chapter.tileMap);
	RenderSystem_Initialize(&game->playState.renderSystem, &game->playState.cBag, &game->playState.chapter.tileMap);
	StatSystem_Initialize(&game->playState.statSystem, &game->playState.cBag);
	return true;
}


//--------------------------------------------------------------------
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

	// Initialize states
	LoadIntroStateAssets(game);
	LoadTitleStateAssets(game);
	memset(&game->highScoreState.scores, 0, sizeof(game->highScoreState.scores));
	game->playState.cBag.freed = true;
	game->playState.loaded = false;

	// Enter title state
	game->gameState = GameState_Intro;
	game->running = true;
	return true;
}


//--------------------------------------------------------------------
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

	SDL_RenderClear(game->renderer);
	if (background) {
	  RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background, SDL_FLIP_NONE);
	}
	if (fader) { 
		RenderSystem_Render_xywh(game->renderer, 0, 0, menuOverlay->w, menuOverlay->h, NULL,menuOverlay, SDL_FLIP_NONE);
		SDL_SetTextureAlphaMod(fader->sdltexture, (game->introState.alpha * 255));
		RenderSystem_Render_xywh(game->renderer, 0, 0, fader->w, fader->h, NULL,  fader, SDL_FLIP_NONE);
	}
	SDL_RenderPresent(game->renderer);
}

//--------------------------------------------------------------------
void UpdateTitle(Game* game, bool* keysdown, bool* keysup, uint32 elapsed) {
	(void) elapsed;

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
			  game->playState.loaded = LoadPlayStateAssets(game);
				SDL_Delay(1000);
			  if (game->playState.loaded)
			  	game->gameState = GameState_Play;
			  else
			  	std::cerr << "Error: Unable to load play state assets." << std::endl;
			  break;
			case 1:
			  game->gameState = GameState_HighScore;
			  LoadHighScoreStateAssets(game);
			  break;
			case 2:
			  break;
			case 3:
			  Mix_HaltMusic();
			  game->gameState = GameState_Closing;
			  break;
		}
	}

	// Render
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background, SDL_FLIP_NONE);
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
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, selection->w, selection->h, NULL, selection, SDL_FLIP_NONE);
	}
	SDL_RenderPresent(game->renderer);
}


//--------------------------------------------------------------------
void UpdateHighScore(Game* game, bool* keysdown, uint32 elapsed) {
	(void) elapsed;
	(void) keysdown;

	// Render
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	SDL_RenderClear(game->renderer);
	if (background) RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background, SDL_FLIP_NONE);
	for (int highScoreIndex = 0; highScoreIndex < Constants::MaxHighScores_; highScoreIndex++) {
		std::string name = "high_score_";
		name.append(std::to_string(game->highScoreState.scores[highScoreIndex]));
		Texture* score = TextureCache_GetTexture(name.c_str());
		int renderX = Constants::ScreenWidth_ / 2 - score->w / 2;
		int renderY = highScoreIndex * (Constants::ScreenHeight_ / Constants::MaxHighScores_);
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, score->w, score->h, NULL, score, SDL_FLIP_NONE);
	}
	SDL_RenderPresent(game->renderer);
}


//--------------------------------------------------------------------
void UpdatePause(Game* game, uint32 elapsed) {
	// TO-DO: Implement some sort of pause
	(void) elapsed;
	(void) game;
}


//--------------------------------------------------------------------
void UpdateWin(Game* game, bool* keysdown) {
	(void) keysdown;
	// Render
	Texture* background = TextureCache_GetTexture(Constants::WinBackground_);
	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background, SDL_FLIP_NONE);
	SDL_RenderPresent(game->renderer);
}


//-------------------------------------------------------------------
void UpdateLose(Game* game, bool* keysdown) {
	(void) keysdown;
	//Render
	Texture* background = TextureCache_GetTexture(Constants::LoseBackground_);
	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	SDL_RenderPresent(game->renderer);
}


//-------------------------------------------------------------------
void UpdateReturn(Game* game) {
	if (game->playState.loaded) {
		if (!game->playState.cBag.freed) {
			ComponentBag_Free(&game->playState.cBag);
		}
		EntityCache_RemoveAll();
		game->playState.loaded = false;
	}
	game->gameState = GameState_Title;
}


//--------------------------------------------------------------------
void UpdatePlay(Game* game, bool* keysdown, float delta) {
	// ComponentBag_Check(&game->playState.cBag); For debugging purposes!
	InputSystem_Update(&game->playState.inputSystem, keysdown);
	PhysicsSystem_Update(&game->playState.physicsSystem, delta);
	AISystem_Update(&game->playState.aiSystem, delta);
	MovementSystem_Update(&game->playState.movementSystem, delta);
	StatSystem_Update(&game->playState.statSystem, delta);
}

void RenderPlay(Game* game, Uint32 elapsed) {
	CameraSystem_Update(&game->playState.cameraSystem);
	RenderSystem_Update(&game->playState.renderSystem, game->renderer, elapsed);
}


//--------------------------------------------------------------------
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
	float delta;

	// Input variables
	bool keysdown[Constants::NumKeys_];
	memset(&keysdown, 0, sizeof(keysdown));
	bool keysup[Constants::NumKeys_];
	memset(&keysup, 0, sizeof(keysup));

	// Begin game loop
	while (game->running) {
		// Calculate timestep
		currentTime = SDL_GetTicks();
		elapsed = currentTime - lastTime;
		lastTime = currentTime;
		lag += elapsed;
		// Poll for input
		while (SDL_PollEvent(&event) != 0) {

			if (event.type == SDL_QUIT) {
				game->running = false;
				Game_Close(game);
				return;
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
				game->gameState = GameState_Closing;
				return;
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_m) {
				if (game->gameState == GameState_Play || game->gameState == GameState_Lose)
					game->gameState = GameState_Returning;
				else
					game->gameState = GameState_Title;
			}
			if (event.type == SDL_KEYDOWN) {
				keysdown[event.key.keysym.sym % Constants::NumKeys_] = true;
				keysup[event.key.keysym.sym % Constants::NumKeys_] = false;
			}
			if (event.type == SDL_KEYUP) {
				keysdown[event.key.keysym.sym % Constants::NumKeys_] = false;
				keysup[event.key.keysym.sym % Constants::NumKeys_] = true;
			}

		}

		// Update game state
		while (lag > Constants::OptimalTime_) {
			delta = 1000.f / (lag / Constants::OptimalTime_);
			switch(game->gameState) {
				case GameState_Title:
					UpdateTitle(game, keysdown, keysup, elapsed);
					break;
				case GameState_Play:
					UpdatePlay(game, keysdown, delta);
					break;
				case GameState_HighScore:
					UpdateHighScore(game, keysdown, elapsed);
					break;
				case GameState_Pause:
					UpdatePause(game, elapsed);
					break;
				case GameState_Win:
					UpdateWin(game, keysdown);
					break;
	      		case GameState_Lose:
	        		UpdateLose(game, keysdown);
					break;
				case GameState_Returning:
					UpdateReturn(game);
					break;
				case GameState_Closing:
					game->running = false;
					Game_Close(game);
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
				break;
			case GameState_Play:
				RenderPlay(game, elapsed);
				break;
			case GameState_HighScore:
				break;
			case GameState_Pause:
				break;
			case GameState_Win:
				break;
      		case GameState_Lose:
				break;
			default:
				break;
		}
	}
}

//--------------------------------------------------------------------
void Game_Close(Game* game) {
	TTF_CloseFont(game->playState.scoreFont);
	TTF_CloseFont(game->playState.healthFont);
	Mix_FreeMusic(game->titleState.titleMusic);
	TextureCache_Free();
	EntityCache_Free();
	Mix_CloseAudio();
	TTF_Quit();
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}
