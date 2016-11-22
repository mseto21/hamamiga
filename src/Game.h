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
#include "GoalSystem.h"
#include "SoundSystem.h"
#include "KillSystem.h"
#include "BulletSystem.h"

// Forward Declarations
struct SDL_Window;
struct SDL_Renderer;
struct _TTF_Font;
struct _Mix_Music;

struct CutScene {
	uint16 slideCount;
	uint16 current;
	Texture* slides[MaxSlides_];
};

enum GameState {
	GameState_Intro = 0,
	GameState_Title,
	GameState_ZoneIntro,
	GameState_LoadPlay,
	GameState_LoadTutorial,
	GameState_Play,
	GameState_HighScore,
	GameState_Options,
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
	_Mix_Music* titleMusic;
};

struct HighScoreState {
	int scores[Constants::MaxHighScores_];
	_TTF_Font* font;
};

struct OptionState {
	uint16 musicVolume = 128;
	float windowBrightness = 1.0f;
	const char* selectionStrings[Constants::TitleScreenSelections_];
	uint8 selection;
	_TTF_Font* font;
};


struct ZoneIntroState {
	float alpha;
	uint32 elapsed;
	_TTF_Font* font;

	bool sliding;
	CutScene startScene;
	CutScene endScene;
};

struct PlayState {
	/* Fonts */
	_TTF_Font* scoreFont;
	_TTF_Font* healthFont;

	bool loaded; /* Ensure file loaded properly. */
	uint8 currentLevel; /* Keeps track of player progress. TO-DO: Read from file! */

	Zone chapter;/* The current level, loaded by FileLoader. */
	ComponentBag cBag;

	/* Systems */
	AISystem aiSystem;
	CameraSystem cameraSystem;
	InputSystem inputSystem;
	MovementSystem movementSystem;
	PhysicsSystem physicsSystem;
	RenderSystem renderSystem;
	GoalSystem goalSystem;
	SoundSystem soundSystem;
	KillSystem killSystem;
	BulletSystem bulletSystem;
};

struct PauseState {
	Texture** pauseTextures;
	int pauseIndex;
};

struct Game {
	// Game globals
	bool running;
	SDL_Window* window;
	SDL_Renderer* renderer;
	GameState gameState;

	// State structs to keep things coupled correctly
	ZoneIntroState zoneIntroState;
	IntroState introState;
	TitleState titleState;
	HighScoreState highScoreState;
	PlayState playState;
	OptionState optionState;
	PauseState pauseState;
};

bool Game_Initialize(Game* game);
void Game_RunLoop(Game* game);
void Game_Close(Game* game);
void Game_TriggerPause(Game* game, Texture** pauseTextures);


