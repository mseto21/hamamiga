struct SDL_Window;
struct SDL_Renderer;

struct Game {
	bool running;
	SDL_Window* window;
	SDL_Renderer* renderer;
};

bool Game_Initialize(Game* game);
void Game_RunLoop(Game* game);
void Game_Close(Game* game);