#include <vector>
struct SDL_Window;
struct SDL_Renderer;

struct Game {
	bool running;
	SDL_Window* window;
	SDL_Renderer* renderer;
        vector<GameState*> states;
};

bool Game_Initialize(Game* game);
void Game_RunLoop(Game* game);
void Game_Close(Game* game);
void changeState(GameState* state);
void pushState(GameState* state);
void popState(GameState* state);

void handleEvents();
void update();
void draw();
