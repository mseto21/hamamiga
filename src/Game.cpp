#include "Game.h"
#include "Timer.h"
#include "InputSystem.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "TextureCache.h"
#include "EntityCache.h"

#include "RectangleComponent.h"
#include "MovementComponent.h"
#include "TextureComponent.h"
#include "InputComponent.h"
#include "AnimationComponent.h"

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

	TextureCache_CreateTexture(game->renderer, "assets/title.png", Constants::TitleBackground_);
	TextureCache_CreateTexture(game->renderer, "assets/fader.png", Constants::TitleFader_);
}


//--------------------------------------------------------------------
void LoadTitleStateAssets(Game* game) {
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
}


//--------------------------------------------------------------------
void LoadPlayStateAssets(Game* game) {
	// Initialize Components
	game->playState.rectangleComponent = (RectangleComponent*)malloc(sizeof(*game->playState.rectangleComponent));
	game->playState.movementComponent = (MovementComponent*)malloc(sizeof(*game->playState.movementComponent));
	game->playState.textureComponent = (TextureComponent*)malloc(sizeof(*game->playState.textureComponent));
	game->playState.inputComponent = (InputComponent*)malloc(sizeof(*game->playState.inputComponent));
	game->playState.animationComponent = (AnimationComponent*)malloc(sizeof(*game->playState.animationComponent));
	Component_Initialize(game->playState.rectangleComponent);
	Component_Initialize(game->playState.movementComponent);
	Component_Initialize(game->playState.textureComponent);
	Component_Initialize(game->playState.inputComponent);
	Component_Initialize(game->playState.animationComponent);
}


//--------------------------------------------------------------------
void LoadHighScoreStateAssets(Game* game) {
	(void)game;
}


//--------------------------------------------------------------------
bool Game_Initialize(Game* game) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}

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

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) && imgFlags)) {
		std::cerr << "Image library could not initialize! SDL Error: " << IMG_GetError() << std::endl;
		return false;
	}

	if (TTF_Init() == -1) {
		std::cerr << "TTF library could not initialize! SDL Error: " << TTF_GetError() << std::endl;
		return false;
	}

	if (TextureCache_GetCache() == NULL) {
		std::cerr << "Error: The texture cache failed to initialize!" << std::endl;
		return false;
	}

	if (EntityCache_GetCache() == NULL) {
		std::cerr << "Error: The entity cache failed to initialize!" << std::endl;
		return false;
	}

	// Initialize States
	LoadIntroStateAssets(game);
	LoadTitleStateAssets(game);
	LoadPlayStateAssets(game);
	LoadHighScoreStateAssets(game);

	// Enter title state
	game->gameState = GameState_Intro;
	game->running = true;
	return true;
}


//--------------------------------------------------------------------
void UpdateIntro(Game* game, float delta) {
	game->introState.time += delta;
	game->introState.alpha = 1 - (game->introState.time / Constants::IntroTime_);
	if ( game->introState.time  >= Constants::IntroTime_) {
		game->gameState = GameState_Title;
	}

	Texture* fader = TextureCache_GetTexture(Constants::TitleFader_);
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);

	SDL_RenderClear(game->renderer);
	if (background) {
		RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, background);
	}

	if (fader) { 
		SDL_SetTextureAlphaMod(fader->sdltexture, (game->introState.alpha * 255));
		RenderSystem_Render_xywh(game->renderer, 0, 0, fader->w, fader->h, fader);
	}
	SDL_RenderPresent(game->renderer);
}


//--------------------------------------------------------------------
void UpdateTitle(Game* game, bool* keysdown, bool* keysup, float delta) {
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

	if (keysdown[SDLK_RETURN]) {
		switch (game->titleState.selection) {
			case 0:
			  game->gameState = GameState_Play;
			  break;
			case 1:
			  game->gameState = GameState_HighScore;
			  break;
			case 2:
			  break;
			case 3:
			  Game_Close(game);
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
	// Update systems
	InputSystem_Update(keysdown, game->playState.inputComponent, game->playState.movementComponent);
	MovementSystem_Update(delta, game->playState.movementComponent, game->playState.rectangleComponent);
	RenderSystem_Update(game->renderer, delta, game->playState.textureComponent, game->playState.rectangleComponent, game->playState.animationComponent);
}


//--------------------------------------------------------------------
void UpdateHighScore(Game* game, float delta) {
	
}


//--------------------------------------------------------------------
void UpdatePause(Game* game, float delta) {
	
}


//--------------------------------------------------------------------
void Game_RunLoop(Game* game) {
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


	// This will be moved elsewhere eventually
	Entity* player = EntityCache_GetNewEntity();
	if (player == nullptr) {
		std::cerr << "Error: The player could not be initialized." << std::endl;
		return;
	}
	InputComponent_Add(game->playState.inputComponent, player->eid);
	RectangleComponent_Add(game->playState.rectangleComponent, player->eid, 50, 0, 32, 32);
	MovementComponent_Add(game->playState.movementComponent, player->eid, 10, 10, 0, 0);
	TextureCache_CreateTexture(game->renderer, "/assets/player.png", "player");
	Texture* playerTexture = TextureCache_GetTexture("player");
	if (playerTexture == nullptr) {
		std::cerr << "Error: The player's texture could not be initialized." << std::endl;
		return;
	}
	TextureComponent_Add(game->playState.textureComponent, player->eid, playerTexture);

	Entity* player2 = EntityCache_GetNewEntity();
	if (player2 == nullptr) {
		std::cerr << "Error: The player could not be initialized." << std::endl;
		return;
	}
	InputComponent_Add(game->playState.inputComponent, player2->eid);
	RectangleComponent_Add(game->playState.rectangleComponent, player2->eid, 0, 0, 32, 32);
	MovementComponent_Add(game->playState.movementComponent, player2->eid, 10, 10, 0, 0);
	TextureCache_CreateTexture(game->renderer, "/assets/player.png", "player2");
	Texture* playerTexture2 = TextureCache_GetTexture("player");
	if (playerTexture2 == nullptr) {
		std::cerr << "Error: The player's texture could not be initialized." << std::endl;
		return;
	}
	TextureComponent_Add(game->playState.textureComponent, player2->eid, playerTexture2);


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
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
				game->running = false;
			}
			if (event.type == SDL_KEYDOWN) {
				keysdown[event.key.keysym.sym] = true;
				keysup[event.key.keysym.sym] = false;
			}
			if (event.type == SDL_KEYUP) {
				keysdown[event.key.keysym.sym] = false;
				keysup[event.key.keysym.sym] = true;
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
				UpdateHighScore(game, delta);
				break;
			case GameState_Pause:
				UpdatePause(game, delta);
				break;
			default:
				break;
		}
	}
}


//--------------------------------------------------------------------
void Game_Close(Game* game) {
	free(game->playState.rectangleComponent);
	free(game->playState.movementComponent);
	free(game->playState.textureComponent);
	free(game->playState.inputComponent);
	free(game->playState.animationComponent);
	TextureCache_Free();
	EntityCache_Free();
	RenderSystem_Free(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}