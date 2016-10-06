#include "Constants.h"
#include "Types.h"

// Forward Declarations
struct SDL_Window;
struct SDL_Renderer;
struct _TTF_Font;

struct RectangleComponent;
struct MovementComponent;
struct TextureComponent;
struct InputComponent;
struct AnimationComponent;

enum GameState {
	GameState_Intro = 0,
	GameState_Title,
	GameState_Play,
	GameState_HighScore,
	GameState_Pause,
};

struct IntroState {
	float alpha;
	float time;
};

struct TitleState {
	const char* selectionStrings[Constants::TitleScreenSelections_];
	uint8 selection;
	_TTF_Font* titleFont;
	bool w;
	bool s;
};

struct HighScoreState {

};

struct PlayState {
	// Components
	RectangleComponent* rectangleComponent;
	MovementComponent* 	movementComponent;
	TextureComponent*   textureComponent;
	InputComponent* inputComponent;
	AnimationComponent* animationComponent;
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