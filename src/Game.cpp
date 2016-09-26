
#include "Game.h"
#include "constants.h"
#include "Player.h"
#include "TextureCache.h"
#include "Enemy.h"

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

const char* PLAYER_IMG = "assets/player.png";
const char* ENEMY_IMG = "assets/enemy.png";

//global textures
SDL_Texture* playerTexture = NULL;
int playerW;
int playerH;

SDL_Texture* enemyTextures[3];
int enemyW;
int enemyH;


/**
 *  Load a texture from an image. converts imagePath -> surface -> texture.
 *
 *  @param path     The path for the image.
 *  @param width    The final width of the surface created
 *  @param height   The final height of the surface created
 *  @param renderer The renderer
 *
 *  @return The texture, with dimensions stored in width and height variables.
 */
SDL_Texture* loadTexture(const char* path, int *width, int *height, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL) {
        std::cout << "Unable to load image: " << path << std::endl;
        return NULL;
    }
    
    //store the dimensions for the surface
    *width = loadedSurface->w;
    *height = loadedSurface->h;
    
    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(newTexture == NULL) {
        std::cout << "Unable to create texture from: " << path << std::endl;
        return NULL;
    }
    
    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );
    return newTexture;
}


bool Game_Initialize(Game* game) {
	game->running = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}

	game->window = SDL_CreateWindow("Fat Hactory", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		Constants::ScreenWidth_, Constants::ScreenHeight_, SDL_WINDOW_SHOWN);
	if (!game->window ) {
		return false;
	}

	if (!Renderer_Initialize(&game->renderer, game->window)) {
		return false;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) && imgFlags)) {
		// handle error
		return false;
	}

	if (TextureCache_GetCache() == NULL) {
		std::cerr << "Error: The texture cache failed to initialize!" << std::endl;
		return false;
	}

	return true;
}

void Game_RunLoop(Game* game) {

	/***** CREATE TEXTURE HERE *****/

	//create textures
    playerTexture = loadTexture(PLAYER_IMG, &playerW, &playerH, game->renderer.renderer);
    enemyTextures[0] = loadTexture(ENEMY_IMG, &enemyW, &enemyH, game->renderer.renderer);
    enemyTextures[1] = loadTexture(ENEMY_IMG, &enemyW, &enemyH, game->renderer.renderer);
    enemyTextures[2] = loadTexture(ENEMY_IMG, &enemyW, &enemyH, game->renderer.renderer);

    //create rects
    Coord2D topLeft = {0,0};
    Coord2D topRight = { 0, Constants::ScreenHeight_-enemyH };
    Coord2D bottomLeft = { Constants::ScreenWidth_-enemyW, 0 };
    Coord2D bottomRight = { Constants::ScreenWidth_-enemyW, Constants::ScreenHeight_-enemyH };

    SDL_Rect playerRect = {topLeft.x, topLeft.y, playerW, playerH };
    SDL_Rect enemyRects[3];
    enemyRects[0] = { topRight.x, topRight.y, enemyW, enemyH };
    enemyRects[1] = { bottomLeft.x, bottomLeft.y, enemyW, enemyH };
    enemyRects[2] = { bottomRight.x, bottomRight.y, enemyW, enemyH };

    Enemy enemies[3];
    enemies[0] = Enemy(enemyTextures[0], enemyRects[0], topRight);
    enemies[1] = Enemy(enemyTextures[1], enemyRects[1], bottomLeft);
    enemies[2] = Enemy(enemyTextures[2], enemyRects[2], bottomRight);

	/***************************/

	SDL_Event event;

	Timer_Initialize(&game->timer); // Initialize game time
	Timer_Start(&game->timer);

	Timer frameTime;	// Create frame timer
	Timer_Initialize(&frameTime);
	int frames = 0;

	// TO-DO: Make this less hacky
	TextureCache_CreateTexture("assets/player.png", game->renderer.renderer);
	Player player;
	player.texture = TextureCache_GetTexture("assets/player.png");
	//std::cout << player.texture << std::endl;

	while (game->running) {
		// Calculate frame time
		float timestep = Timer_GetTicks(&frameTime) / 1000.f;
		Timer_Start(&frameTime);
		float avgFPS = frames / (Timer_GetTicks(&game->timer) / 1000.f);
		if (avgFPS > 2000000) {
			avgFPS = 0;
		}
		Timer_Start(&game->timer);
		// Poll input
		while (SDL_PollEvent(&event) != 0) {
			player.GetInput(&event);
			if (event.type == SDL_QUIT) {
				game->running = false;
			}
			if (event.type == SDL_KEYUP &&  event.key.keysym.sym == 
				SDLK_ESCAPE){
				game->running = false;
			}
		}

		// Update
		player.Update(timestep);

		// Render
		 Renderer_RenderCoord(&game->renderer, &player.position, player.texture);
	 Renderer_CompleteRender(&game->renderer);

		/***** RENDER HERE *****/

		//render images
        SDL_RenderClear(game->renderer.renderer);
        SDL_RenderCopy(game->renderer.renderer, playerTexture, NULL, &playerRect);
        for (int i = 0; i < 3; i++) {
        	SDL_RenderCopy(game->renderer.renderer, enemyTextures[i], NULL, &enemyRects[i]);
        }
        SDL_RenderPresent(game->renderer.renderer);

        /************************/

		++frames;

		playerRect.x = player.position.x;
		playerRect.y = player.position.y;

		for (int i = 0; i < 3; i++) {
			enemies[i].move();
			enemyRects[i].x = enemies[i].position.x;
			enemyRects[i].y = enemies[i].position.y;
		}
	}
}

void Game_Close(Game* game) {
	TextureCache_Free();
	Renderer_Free(&game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}