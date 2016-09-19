#include "Timer.hpp"
#include <SDL.h>

struct Game {
	bool running;
	Timer timer;
	SDL_Window* window;
	SDL_Renderer * renderer;
};

bool Game_Initialize(Game* game);
void Game_RunLoop(Game* game);
void Game_Close(Game* game);