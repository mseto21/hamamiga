#include "Game.h"
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
#include "PhysicsComponent.h"
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

	TextureCache_CreateTexture(game->renderer, "assets/title.png", Constants::TitleBackground_);
	TextureCache_CreateTexture(game->renderer, "assets/fader.png", Constants::TitleFader_);

	TextureCache_CreateTexture(game->renderer, "assets/background.png", Constants::GameBackground_);
	//Later make own fcn for win screen
	TextureCache_CreateTexture(game->renderer, "assets/win.png", Constants::WinBackground_);
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
	// Initialize Components
	game->playState.rectangleComponent = (RectangleComponent*)malloc(sizeof(*game->playState.rectangleComponent));
	game->playState.movementComponent = (MovementComponent*)malloc(sizeof(*game->playState.movementComponent));
	game->playState.textureComponent = (TextureComponent*)malloc(sizeof(*game->playState.textureComponent));
	game->playState.inputComponent = (InputComponent*)malloc(sizeof(*game->playState.inputComponent));
	game->playState.animationComponent = (AnimationComponent*)malloc(sizeof(*game->playState.animationComponent));
	game->playState.physicsComponent = (PhysicsComponent*)malloc(sizeof(*game->playState.physicsComponent));
	game->playState.healthComponent = (HealthComponent*)malloc(sizeof(*game->playState.healthComponent));
	Component_Initialize(game->playState.rectangleComponent);
	Component_Initialize(game->playState.movementComponent);
	Component_Initialize(game->playState.textureComponent);
	Component_Initialize(game->playState.inputComponent);
	Component_Initialize(game->playState.animationComponent);
	Component_Initialize(game->playState.healthComponent);

	// Initialize variables
	game->playState.score = 0;

	// Initialize all entities
	Entity* player = EntityCache_GetNewEntity();
	if (player == nullptr) {
		std::cerr << "Error: The player could not be initialized." << std::endl;
		return;
	}
	TextureCache_CreateTexture(game->renderer, "assets/background.png", "game_background");

	//Creating animation
	Animation playerAnimation;
	Animation_Initialize(&playerAnimation, 4, 10.f, Constants::PlayerWSize_, Constants::PlayerHSize_);

	//Component Adding
	InputComponent_Add(game->playState.inputComponent, player->eid);
	RectangleComponent_Add(game->playState.rectangleComponent, player->eid, 50, 0, Constants::PlayerWSize_, Constants::PlayerHSize_);
	MovementComponent_Add(game->playState.movementComponent, player->eid, 10, 10, 0, 0);
	TextureCache_CreateTexture(game->renderer, "assets/tinykev.png", "player");
	if (TextureCache_GetTexture("player") == nullptr) {
		std::cerr << "Error: The player's texture could not be initialized." << std::endl;
	}
	TextureComponent_Add(game->playState.textureComponent, player->eid, TextureCache_GetTexture("player"));
	HealthComponent_Add(game->playState.healthComponent, player->eid, 100);
	AnimationComponent_Add(game->playState.animationComponent, player->eid, &playerAnimation);
	//Win state for now
	Entity* trophy = EntityCache_GetNewEntity();
	RectangleComponent_Add(game->playState.rectangleComponent, trophy->eid, 500, 350, 30, 17);
	TextureCache_CreateTexture(game->renderer, "assets/crown.png", "trophy");
	if (TextureCache_GetTexture("trophy") == nullptr) {
		std::cerr << "Error: The trophy's texture could not be initialized." << std::endl;
	}
	TextureComponent_Add(game->playState.textureComponent, trophy->eid, TextureCache_GetTexture("trophy"));

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

	// Initialize current states
	LoadIntroStateAssets(game);
	LoadTitleStateAssets(game);

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

	if (keysdown[SDLK_RETURN]) {
		switch (game->titleState.selection) {
			case 0:
				Mix_HaltMusic(); // Stop Playing any music.
			  game->gameState = GameState_Play;
			  LoadPlayStateAssets(game);
			  break;
			case 1:
			  //Mix_HaltMusic();
			  game->gameState = GameState_HighScore;
			  LoadHighScoreStateAssets(game);
			  break;
			case 2:
			  break;
			case 3:
				Mix_HaltMusic();
			  Game_Close(game);
			  break;
		}
	}

	// Check for music playing
	if (Mix_PlayingMusic() == 0) {
		Mix_PlayMusic(game->titleState.titleMusic, 0);
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
	// Process Input
	if (keysdown[SDLK_m] == true) {
		game->gameState = GameState_Title;
		game->playState.score = 0; // Don't save their high score if they quit early
	}

	//Checking if the player has reached the trophy
	Rectangle* rect = &game->playState.rectangleComponent->entityRectangles[0];//hax with 0
	//std::cout << rect->x << "\n";
	if ((rect->x+rect->w) > 500 && rect->x < 530){
		if ((rect->y+rect->h) > 350 && rect->y < 367){
			std::cout << "WIN!";
			game->gameState = GameState_Win;
		}
	}//hard code 500x, 350y

	// If game ends regularly...
		// update game->highScoreState.scores

	// Update systems
	InputSystem_Update(keysdown, game->playState.inputComponent, game->playState.movementComponent);
	MovementSystem_Update(delta, game->playState.movementComponent, game->playState.rectangleComponent);
	Texture* background = TextureCache_GetTexture("game_background"); 
	SDL_RenderClear(game->renderer);
	if (background) {
		Rectangle backgroundRect = {0, 0, background->w, background->h};
		RenderSystem_RenderCoord(game->renderer, &backgroundRect, NULL, background);
	}

	RenderSystem_Update(game->renderer, delta, game->playState.textureComponent, game->playState.rectangleComponent, game->playState.animationComponent);
	SDL_RenderPresent(game->renderer);
}


//--------------------------------------------------------------------
void UpdateHighScore(Game* game, bool* keysdown, float delta) {
	(void) delta;

	if (keysdown[SDLK_m] == true) {
		game->gameState = GameState_Title;
	}

	// Render
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	SDL_RenderClear(game->renderer);
	if (background) RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, background);
	for (int highScoreIndex = 0; highScoreIndex < Constants::MaxHighScores_; highScoreIndex++) {
		std::string name = "high_score_";
		name.append(std::to_string(game->highScoreState.scores[highScoreIndex]));
		Texture* score = TextureCache_GetTexture(name.c_str());
		if (!score) {
			std::cerr << "Could not find texture for score " << game->highScoreState.scores[highScoreIndex] << std::endl;
			continue;
		}
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
	if (keysdown[SDLK_m] == true) {
		game->gameState = GameState_Title;
	}
	// Render
	Texture* background = TextureCache_GetTexture(Constants::WinBackground_);
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
				UpdateHighScore(game, keysdown, delta);
				break;
			case GameState_Pause:
				UpdatePause(game, delta);
				break;
			case GameState_Win:
				UpdateWin(game, keysdown);
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
	free(game->playState.physicsComponent);
	free(game->playState.healthComponent);

	Mix_FreeMusic(game->titleState.titleMusic);
	TextureCache_Free();
	EntityCache_Free();
	Mix_CloseAudio();
	TTF_Quit();
	RenderSystem_Free(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}
