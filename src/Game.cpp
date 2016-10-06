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
void LoadIntroScreenAssets(Game* game) {
	game->introScreen.time = 0.f;
	game->introScreen.alpha = 0.f;

	TextureCache_CreateTexture(game->renderer, "assets/title.png", Constants::TitleBackground_);
	TextureCache_CreateTexture(game->renderer, "assets/fader.png", Constants::TitleFader_);
}

//--------------------------------------------------------------------
void LoadTitleScreenAssets(Game* game) {
	game->titleScreen.selection = 0;
	game->titleScreen.titleFont = TTF_OpenFont("assets/minnie\'shat.ttf", 75);
	if (!game->titleScreen.titleFont) {
		std::cerr << "Unable to initialize the font! SDL_Error: " << TTF_GetError() << std::endl;
		return;
	}
	TTF_SetFontHinting(game->titleScreen.titleFont, TTF_HINTING_MONO);
	game->titleScreen.selectionStrings[0] = "Play";
	game->titleScreen.selectionStrings[1] = "High Scores";
	game->titleScreen.selectionStrings[2] = "Options";
	game->titleScreen.selectionStrings[3] = "Quit";

	SDL_Color baseColor = {255, 255, 255, 255};
	SDL_Color selectedColor = {100, 100, 100, 100};
	for (int selectionIndex = 0; selectionIndex < Constants::TitleScreenSelections_; selectionIndex++) {
		std::string base = game->titleScreen.selectionStrings[selectionIndex];
		base.append("_base");
		TextureCache_CreateFont(game->renderer, game->titleScreen.titleFont, baseColor, game->titleScreen.selectionStrings[selectionIndex], base.c_str());
		std::string select = game->titleScreen.selectionStrings[selectionIndex];
		select.append("_select");
		TextureCache_CreateFont(game->renderer, game->titleScreen.titleFont, selectedColor, game->titleScreen.selectionStrings[selectionIndex], select.c_str());
	}
	TTF_CloseFont(game->titleScreen.titleFont);
}

//--------------------------------------------------------------------
void LoadPlayScreenAssets(Game* game) {
	// Initialize Components
	game->rectangleComponent = (RectangleComponent*)malloc(sizeof(*game->rectangleComponent));
	game->movementComponent = (MovementComponent*)malloc(sizeof(*game->movementComponent));
	game->textureComponent = (TextureComponent*)malloc(sizeof(*game->textureComponent));
	game->inputComponent = (InputComponent*)malloc(sizeof(*game->inputComponent));
	Component_Initialize(game->rectangleComponent);
	Component_Initialize(game->movementComponent);
	Component_Initialize(game->textureComponent);
	Component_Initialize(game->inputComponent);
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

	// Initialize States and load currently necessary assets
	LoadIntroScreenAssets(game);
	LoadTitleScreenAssets(game);
	LoadPlayScreenAssets(game);

	// Enter title state
	game->gameState = GameState_Intro;
	game->running = true;
	return true;
}

//--------------------------------------------------------------------
void UpdateIntro(Game* game, float delta) {
	game->introScreen.time += delta;
	game->introScreen.alpha = 1 - (game->introScreen.time / Constants::IntroTime_);
	if ( game->introScreen.time  >= Constants::IntroTime_) {
		game->gameState = GameState_Title;
	}

	Texture* fader = TextureCache_GetTexture(Constants::TitleFader_);
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);

	SDL_RenderClear(game->renderer);
	if (background) {
		RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, background);
	}

	if (fader) { 
		SDL_SetTextureAlphaMod(fader->sdltexture, (game->introScreen.alpha * 255));
		RenderSystem_Render_xywh(game->renderer, 0, 0, fader->w, fader->h, fader);
	}
	SDL_RenderPresent(game->renderer);
}

//--------------------------------------------------------------------
void UpdateTitle(Game* game, bool* keysdown, bool* keysup, float delta) {
	// Update their options
	if (keysdown[SDLK_w] && !game->titleScreen.w) {
		if (!game->titleScreen.w) {
			game->titleScreen.w = true;
			game->titleScreen.s = false;
			game->titleScreen.selection--;
			game->titleScreen.selection %= Constants::TitleScreenSelections_;
		}
	}
	if (keysup[SDLK_w]) {
		game->titleScreen.w = false;
	}

	if (keysdown[SDLK_s] && !game->titleScreen.s) {
		game->titleScreen.w = false;
		game->titleScreen.s = true;
		game->titleScreen.selection++;
		game->titleScreen.selection %= Constants::TitleScreenSelections_;
	}
	if (keysup[SDLK_s]) {
		game->titleScreen.s = false;
	}

	if (keysdown[SDLK_RETURN]) {
		switch (game->titleScreen.selection) {
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

	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, background);
	for (int selectionIndex = 0; selectionIndex < Constants::TitleScreenSelections_; selectionIndex++) {
		Texture* selection;
		if (selectionIndex == game->titleScreen.selection) {
			std::string base = game->titleScreen.selectionStrings[selectionIndex];
			base.append("_base");
			selection = TextureCache_GetTexture(base.c_str());
		} else {
			std::string select = game->titleScreen.selectionStrings[selectionIndex];
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
	InputSystem_Update(keysdown, game->inputComponent, game->movementComponent);
	MovementSystem_Update(delta, game->movementComponent, game->rectangleComponent);
	RenderSystem_Update(game->renderer, game->textureComponent, game->rectangleComponent);
}

//--------------------------------------------------------------------
void UpdateHighScore(Game* game, float delta) {
	
}

//--------------------------------------------------------------------
void UpdatePause(Game* game, float delta) {
	
}

//--------------------------------------------------------------------
void Game_RunLoop(Game* game) {
	SDL_Event event;
	Uint32 currentTime = SDL_GetTicks();
	Uint32 frameTime;
	Uint32 lastTime;
	float delta;

	bool keysdown[Constants::NumKeys_];
	memset(&keysdown, 0, sizeof(keysdown));
	bool keysup[Constants::NumKeys_];
	memset(&keysup, 0, sizeof(keysup));

	Entity* player = EntityCache_GetNewEntity();
	if (player == nullptr) {
		std::cerr << "Error: The player could not be initialized." << std::endl;
		return;
	}

	InputComponent_Add(game->inputComponent, player->eid);
	RectangleComponent_Add(game->rectangleComponent, player->eid, 50, 0, 32, 32);
	MovementComponent_Add(game->movementComponent, player->eid, 10, 10, 0, 0);
	TextureCache_CreateTexture(game->renderer, "/assets/player.png", "player");
	Texture* playerTexture = TextureCache_GetTexture("player");
	if (playerTexture == nullptr) {
		std::cerr << "Error: The player's texture could not be initialized." << std::endl;
		return;
	}
	TextureComponent_Add(game->textureComponent, player->eid, playerTexture);

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
	free(game->rectangleComponent);
	free(game->movementComponent);
	free(game->textureComponent);
	free(game->inputComponent);
	TextureCache_Free();
	EntityCache_Free();
	RenderSystem_Free(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}