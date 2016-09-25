#include "Timer.h"
#include "Renderer.h"
struct SDL_Window;

struct Game {
	bool running;
	Timer timer;
	SDL_Window* window;
	Renderer* renderer;
};

bool Game_Initialize(Game* game);
void Game_RunLoop(Game* game);
void Game_Close(Game* game);