#include "GameState.h"
#include <vector>

struct SDL_Window;
struct SDL_Renderer;

struct Game {
	bool running;
	SDL_Window* window;
	SDL_Renderer* renderer;
        std::vector<GameState*> states;
};

bool Game_Initialize(Game* game);
void Game_RunLoop(Game* game);
void Game_Close(Game* game);
void Game_ChangeState(Game* game, GameState* state);
void Game_PushState(Game* game, GameState* state);
void Game_PopState(Game* game);

void handleEvents(Game* game);
void update(Game* game);
void draw(Game* game);
