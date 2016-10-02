#include "Game.h"
#include "Collision.h"
#include "constants.h"
#include "Player.h"
#include "TextureCache.h"
#include "Enemy.h"
#include "Timer.h"
#include "Renderer.h"

#include <SDL_mixer.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using std::cout;
using std::endl;

Mix_Music* collideSound;
const char* PLAYER_IMG = "assets/player.png";
const char* ANIM_IMG = "assets/playeranim.png";
const char* ENEMY_IMG = "assets/enemy.png";
const char* COLLIDE_SND = "assets/ow.mp3";

const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect playerClips[ WALKING_ANIMATION_FRAMES ];

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

	game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
	if (!game->renderer) {
		std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
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

	//Load media
	//if( !gSpriteSheetTexture.loadFromFile( ANIM_IMG ) )
	//
	//	printf( "Failed to load walking animation texture!\n" );
		//success = false;
	//}
	//else
	//{
		//Set sprite clips
		playerClips[ 0 ].x =   0;
		playerClips[ 0 ].y =   0;
		playerClips[ 0 ].w =  68;
		playerClips[ 0 ].h = 42;

		playerClips[ 1 ].x =  68;
		playerClips[ 1 ].y =   0;
		playerClips[ 1 ].w =  68;
		playerClips[ 1 ].h = 42;
		
		playerClips[ 2 ].x = 136;
		playerClips[ 2 ].y =   0;
		playerClips[ 2 ].w =  68;
		playerClips[ 2 ].h = 42;

		playerClips[ 3 ].x = 204;
		playerClips[ 3 ].y =   0;
		playerClips[ 3 ].w =  68;
		playerClips[ 3 ].h = 42;
	//}

		if( Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096 ) < 0 ) {
    	std::cerr <<"SDL_mixer could not initialize! SDL_mixer Error:"
    			  << Mix_GetError() << "\n";
    }

	return true;
}


void RenderCoord(SDL_Renderer* renderer, Coord2D* point, Texture* texture) {
	if (!renderer) {
		return;
	}
	if (!texture) {
		return;
	}

	SDL_Rect rquad;
	rquad.x = point->x;
	rquad.y = point->y;
	rquad.w = texture->w;
	rquad.h = texture->h;
	SDL_RenderCopy(renderer, texture->sdltexture, NULL, &rquad);
}


void Game_RunLoop(Game* game) {
	SDL_Event event;

	const int MaxEnemies_ = 3;
	Enemy enemies[MaxEnemies_];
    
    Coord2D topRight = {Constants::ScreenWidth_-80, 0};
    Coord2D bottomLeft = {0, Constants::ScreenHeight_-41};
    Coord2D bottomRight = {Constants::ScreenWidth_-80, Constants::ScreenHeight_-41};
    /*
    Coord2D coords[MaxEnemies_] = {
        { Constants::ScreenHeight_, 0 },
        { .x = 0, .y = Constants::ScreenWidth_},
        { .x = Constants::ScreenHeight_, .y = Constants::ScreenWidth_}
    };*/
    Coord2D coords[MaxEnemies_];
    coords[0] = topRight;
    coords[1] = bottomRight;
    coords[2] = bottomLeft;

    for (int i = 0; i < MaxEnemies_; i++) {
        enemies[i] = Enemy(coords[i]);
        enemies[i].texture = TextureCache_CreateTexture(ENEMY_IMG, game->renderer);
        enemies[i].resetMaxPosition();
    }

	// TO-DO: Make this less hacky
	Player player;
	player.texture = TextureCache_CreateTexture(ANIM_IMG, game->renderer);

	Uint32 currentTime = SDL_GetTicks();
	Uint32 frameTime;
	Uint32 lastTime;
	float delta;

	const float TargetFps_ = 60.f;
	const float OptimalTime_ = 1000 / TargetFps_;

	bool keysdown[Constants::NumKeys_];

	//load sound file
	collideSound = Mix_LoadMUS(COLLIDE_SND);

	while (game->running) {
	  handleEvents();
	  update();
	  draw();
		// Calculate timestep
		lastTime = currentTime;
		currentTime = SDL_GetTicks();
		frameTime = currentTime - lastTime;
		delta = frameTime / ((float)OptimalTime_);
		if (frameTime < OptimalTime_) {
			SDL_Delay((OptimalTime_) - frameTime);
		}


		// Get input
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				game->running = false;
			} if (event.type == SDL_KEYDOWN &&  event.key.keysym.sym == SDLK_ESCAPE){
				game->running = false;
			}
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_w:
					case SDLK_a:
					case SDLK_s:
					case SDLK_d:
						keysdown[event.key.keysym.sym] = true;
						break;
					default:
						break;
				}
			}
			if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
					case SDLK_w:
					case SDLK_a:
					case SDLK_s:
					case SDLK_d:
						keysdown[event.key.keysym.sym] = false;
						break;
					default:
						break;
				}
			}
		}
		player.GetInput(keysdown);

	  	// Update entities
		player.Update(delta);
		for (int i = 0; i < MaxEnemies_; i++) {
		  if (Collision::collision(player.position, player.texture->w,
					   player.texture->h, enemies[i].position,
					   enemies[i].texture->w, enemies[i].texture->h)) {
            Mix_PlayMusic(collideSound, 1);
		    player.UndoMove();
            break;
		  }
		}
		for (int i = 0; i < MaxEnemies_; i++) {
			enemies[i].Update(delta);
		}
        for (int i = 0; i < MaxEnemies_; i++) {
            enemies[i].update(delta);
            if (Collision::collision(enemies[i].position, enemies[i].texture->w,
				     enemies[i].texture->h, player.position,
				     player.texture->w, player.texture->h)) {
            		Mix_PlayMusic(collideSound, 1);
                cout << "colliDED WITH PLAYER" << endl;
                enemies[i].undoMove();
                enemies[i].reverseDirection();
            } else {
                for (int j = 0; j < MaxEnemies_; j++) {
                    if (i != j) {
		      if (Collision::collision(enemies[i].position, enemies[i].texture->w,
					       enemies[i].texture->h, enemies[j].position,
					       enemies[j].texture->w,
					       enemies[j].texture->h)) {
                            cout << "colliDED WITH ENEMY" << endl;
                            enemies[i].undoMove();
                            enemies[i].reverseDirection();
                        }
                    }
                }
            }
        }

    uint32 sprite = (currentTime / 100) % 4;
    SDL_Rect * currentClip = &playerClips[sprite];

		// Render
		SDL_RenderClear(game->renderer);
		Renderer_RenderCoord(game->renderer, &player.position, player.texture, currentClip);
		for (int i = 0; i < MaxEnemies_; i++) {
			Renderer_RenderCoord(game->renderer, &enemies[i].position, enemies[i].texture, NULL);
		}
		SDL_RenderPresent(game->renderer);
	}
}

void Game_Close(Game* game) {
    while (!states.empty()) {
        states.back()->Close();
        states.pop_back();
    }
    Mix_FreeMusic(collideSound);
    TextureCache_Free();
    Renderer_Free(game->renderer);
    SDL_DestroyWindow(game->window);
    Mix_Quit();
    SDL_Quit();
}

void changeState(GameState* state) {
    //close current state
    if (!states.empty()) {
      states.back()->close();
      states.pop_back();
    }
    //store and initialize new state
    states.push_back(state);
    states.back()->initialize();
}

void pushState(GameState* state) {
    if (!states.empty()) {
      states.back()->pause();
    }
    states.push_back(state);
    states.back()->initialize();
}

void popState(GameState* state) {
    if (!states.empty()) {
        states.back()->close();
        states.pop_back();
    }
    if (!states.empty()) {
        states.back()->resume();
    }
}

void handleEvents() {
    states.back()->handleEvents(this);
}

void update() {
    states.back()->update(this);
}

void draw() {
    states.back()->draw(this);
}
