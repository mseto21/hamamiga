#include "Game.h"
#include "InputSystem.h"
#include "PhysicsSystem.h"
#include "MovementSystem.h"
#include "RenderSystem.h"

#include "TextureCache.h"
#include "EntityCache.h"

#include "HealthComponent.h"

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
	game->introState.time = 0.f;
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
void LoadPlayStateAssets(Game* game) {
	game->playState.scoreFont = TTF_OpenFont("assets/minnie\'shat.ttf", 30);
	game->playState.healthFont = TTF_OpenFont("assets/minnie\'shat.ttf", 30);
	if (!game->playState.scoreFont) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return;
	}
	if (!game->playState.healthFont) {
    std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
    return;
	}
	TTF_SetFontHinting(game->playState.scoreFont, TTF_HINTING_MONO);
	TTF_SetFontHinting(game->playState.healthFont, TTF_HINTING_MONO);
	ComponentBag_Malloc(&game->playState.cBag);
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
	LoadPlayStateAssets(game);
	memset(&game->highScoreState.scores, 0, sizeof(game->highScoreState.scores));

	// Enter title state
	game->gameState = GameState_Intro;
	game->running = true;
	return true;
}


//--------------------------------------------------------------------
void UpdateIntro(Game* game, float delta) {
	// Check if the intro is done
	game->introState.time += delta;
	game->introState.alpha = 1 - (game->introState.time / Constants::IntroTime_);
	if ( game->introState.time  >= Constants::IntroTime_) {
		game->gameState = GameState_Title;
	}

	// Render intro
	Texture* fader = TextureCache_GetTexture(Constants::TitleFader_);
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	Texture* menuOverlay = TextureCache_GetTexture(Constants::MenuOverlay_);

	SDL_RenderClear(game->renderer);
	if (background) {
		RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, background);
	}
	if (fader) { 
		RenderSystem_Render_xywh(game->renderer, 0, 0, menuOverlay->w, menuOverlay->h, menuOverlay);
		SDL_SetTextureAlphaMod(fader->sdltexture, (game->introState.alpha * 255));
		RenderSystem_Render_xywh(game->renderer, 0, 0, fader->w, fader->h, fader);
	}
	SDL_RenderPresent(game->renderer);
}


//--------------------------------------------------------------------
void UpdateTitle(Game* game, bool* keysdown, bool* keysup, float delta) {
	(void) delta;

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
			  game->gameState = GameState_Play;
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
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, background);
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
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, selection->w, selection->h, selection);
	}
	SDL_RenderPresent(game->renderer);
}


//--------------------------------------------------------------------
void UpdatePlay(Game* game, bool* keysdown, float delta) {
	game->playState.score += delta;

	int* health = &game->playState.cBag.healthComponent->health[Constants::PlayerIndex_]; //hax with 0
	if (*health <= 0) {
	  game->gameState = GameState_Lose;
	}

	// Update systems
	InputSystem_Update(keysdown, game->playState.cBag.inputComponent, game->playState.cBag.movementComponent, game->playState.cBag.rectangleComponent);
	MovementSystem_Update(delta, game->playState.cBag.movementComponent, game->playState.cBag.rectangleComponent);
	PhysicsSystem_Update(delta, game->playState.cBag.physicsComponent, game->playState.cBag.movementComponent, game->playState.cBag.rectangleComponent, game->playState.cBag.healthComponent);
	
	Texture* background = TextureCache_GetTexture("game_background"); 
	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, background);
	RenderSystem_Update(game->renderer, delta, game->playState.cBag.textureComponent, game->playState.cBag.rectangleComponent, game->playState.cBag.animationComponent, game->playState.cBag.movementComponent);
	Texture scoreTexture;
	Texture_CreateTextureFromFont(&scoreTexture, game->renderer, game->playState.scoreFont, {255, 255, 255, 255}, std::to_string(game->playState.score).c_str(), "score");
	RenderSystem_Render_xywh(game->renderer, 0, 0, scoreTexture.w, scoreTexture.h, &scoreTexture);
	Texture healthTexture;
	Texture_CreateTextureFromFont(&healthTexture, game->renderer, game->playState.healthFont, {20, 200, 100, 255}, std::to_string(*health).c_str(), "health");
	RenderSystem_Render_xywh(game->renderer, Constants::ScreenWidth_ - healthTexture.w - 10, 0, healthTexture.w, healthTexture.h, &healthTexture);
	SDL_RenderPresent(game->renderer);
}


//--------------------------------------------------------------------
void UpdateHighScore(Game* game, bool* keysdown, float delta) {
	(void) delta;
	(void) keysdown;

	// Render
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	SDL_RenderClear(game->renderer);
	if (background) RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, background);
	for (int highScoreIndex = 0; highScoreIndex < Constants::MaxHighScores_; highScoreIndex++) {
		std::string name = "high_score_";
		name.append(std::to_string(game->highScoreState.scores[highScoreIndex]));
		Texture* score = TextureCache_GetTexture(name.c_str());
		int renderX = Constants::ScreenWidth_ / 2 - score->w / 2;
		int renderY = highScoreIndex * (Constants::ScreenHeight_ / Constants::MaxHighScores_);
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, score->w, score->h, score);
	}
	SDL_RenderPresent(game->renderer);
}


//--------------------------------------------------------------------
void UpdatePause(Game* game, float delta) {
	// TO-DO: Implement some sort of pause
	(void) delta;
	(void) game;
}

//--------------------------------------------------------------------
void UpdateWin(Game* game, bool* keysdown) {
	(void) keysdown;
	// Render
	Texture* background = TextureCache_GetTexture(Constants::WinBackground_);
	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, background);
	SDL_RenderPresent(game->renderer);
}

//-------------------------------------------------------------------
void UpdateLose(Game* game, bool* keysdown) {
	(void) keysdown;
  //Render
  Texture* background = TextureCache_GetTexture(Constants::LoseBackground_);
  SDL_RenderClear(game->renderer);
  RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, background);
	SDL_RenderPresent(game->renderer);
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
	Uint32 frameTime;
	Uint32 lastTime;
	float delta;

	// Input variables
	bool keysdown[Constants::NumKeys_];
	memset(&keysdown, 0, sizeof(keysdown));
	bool keysup[Constants::NumKeys_];
	memset(&keysup, 0, sizeof(keysup));

	// Begin game loop
	while (game->running) {
		// Calculate timestep
		lastTime = currentTime;
		currentTime = SDL_GetTicks();
		frameTime = currentTime - lastTime;
		delta = frameTime / Constants::OptimalTime_;
		if (frameTime < Constants::OptimalTime_) {
			SDL_Delay((Constants::OptimalTime_) - frameTime);
		}

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
				game->gameState = GameState_Returning;
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

		// Update appropriate game state
		switch(game->gameState) {
			case GameState_Intro:
				UpdateIntro(game, delta);
				break;
			case GameState_Title:
				UpdateTitle(game, keysdown, keysup, delta);
				break;
			case GameState_Play:
				UpdatePlay(game, keysdown, delta);
				break;
			case GameState_HighScore:
				UpdateHighScore(game, keysdown, delta);
				break;
			case GameState_Pause:
				UpdatePause(game, delta);
				break;
			case GameState_Win:
				UpdateWin(game, keysdown);
				break;
      case GameState_Lose:
        UpdateLose(game, keysdown);
				break;
			case GameState_Returning:
				ComponentBag_Reset(&game->playState.cBag);
				EntityCache_RemoveAll();
				game->gameState = GameState_Title;
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

//--------------------------------------------------------------------
void Game_Close(Game* game) {
	ComponentBag_Free(&game->playState.cBag);
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
