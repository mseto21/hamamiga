struct SDL_Window;
struct SDL_Renderer;

struct RectangleComponent;
struct MovementComponent;
struct TextureComponent;
struct InputComponent;

struct Game {
	bool running;
	SDL_Window* window;
	SDL_Renderer* renderer;

	// Components
	RectangleComponent* rectangleComponent;
	MovementComponent* 	movementComponent;
	TextureComponent*   textureComponent;
	InputComponent* inputComponent;
};

bool Game_Initialize(Game* game);
void Game_RunLoop(Game* game);
void Game_Close(Game* game);