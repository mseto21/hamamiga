#include "Constants.h"
#include "Types.h"
#include "ComponentBag.h"
#include "Zone.h"

#include "AISystem.h"
#include "CameraSystem.h"
#include "InputSystem.h"
#include "MovementSystem.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "StatSystem.h"

// Forward Declarations
struct SDL_Window;
struct SDL_Renderer;
struct _TTF_Font;
struct _Mix_Music;


enum GameState {
	GameState_Intro = 0,
	GameState_Title,
	GameState_Play,
	GameState_HighScore,
	GameState_Pause,
	GameState_Win,
	GameState_Lose,
	GameState_Returning,
	GameState_Closing,
};

struct IntroState {
	float alpha;
	uint32 elapsed;
};

struct TitleState {
	const char* selectionStrings[Constants::TitleScreenSelections_];
	uint8 selection;
	_TTF_Font* titleFont;
	bool w;
	bool s;
	_Mix_Music* titleMusic;
};

struct HighScoreState {
	int scores[Constants::MaxHighScores_];
	_TTF_Font* font;
};

struct PlayState {
	// Fonts
	_TTF_Font* scoreFont;
	_TTF_Font* healthFont;

	bool loaded; // Check if the map is loaded

	Zone chapter;/* The current level, loaded by FileLoader. */
	ComponentBag cBag;

	// Systems
	AISystem aiSystem;
	CameraSystem cameraSystem;
	InputSystem inputSystem;
	MovementSystem movementSystem;
	PhysicsSystem physicsSystem;
	RenderSystem renderSystem;
	StatSystem statSystem;
};

struct Game {
	// Game globals
	bool running;
	SDL_Window* window;
	SDL_Renderer* renderer;
	GameState gameState;

	// State structs to keep things coupled correctly
	IntroState introState;
	TitleState titleState;
	HighScoreState highScoreState;
	PlayState playState;
};

bool Game_Initialize(Game* game);
void Game_RunLoop(Game* game);
void Game_Close(Game* game);


