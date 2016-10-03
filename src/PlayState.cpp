#include <stdio.h>

#include "Collision.h"
#include "constants.h"
#include "Player.h"
#include "Enemy.h"
#include "Renderer.h"
#include "Game.h"
#include "PlayState.h"
#include "TextureCache.h"
#include "MenuState.h"
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using std::cout;
using std::endl;

Mix_Music* collideSound;
const char* PLAYER_IMG = "assets/player.png";
const char* ANIM_IMG = "assets/tinykev.png";
const char* ENEMY_IMG = "assets/tinydemon.png";
const char* COLLIDE_SND = "assets/ow.mp3";

const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect playerClips[ WALKING_ANIMATION_FRAMES ];
SDL_Rect enemyClips[ WALKING_ANIMATION_FRAMES ];
const int playerW = 65;
const int playerH = 95;
const int enemyW = 95;
const int enemyH = 95;

const int MaxEnemies_ = 3;
Enemy enemies[MaxEnemies_];

Player player;

Uint32 currentTime = SDL_GetTicks();
Uint32 frameTime;
Uint32 lastTime;
float delta;

const float TargetFps_ = 60.f;
const float OptimalTime_ = 1000 / TargetFps_;

bool keysdown[Constants::NumKeys_];

PlayState PlayState::playState;

void PlayState::initialize(Game* game) {
  //Load media
    //Set sprite clips
    for (int i = 0; i < 4; i++){//change to better var names
      if (i != 0){
        playerClips[i].x = playerClips[i-1].x + playerW;
      } else {
        playerClips[i].x = 0;
      }
      playerClips[i].y = 0;
      playerClips[i].w = playerW;
      playerClips[i].h = playerH;
    }
  //Set enemy sprite
    for (int i = 0; i < 4; i++){//change to better var names
      if (i != 0){
        enemyClips[i].x = enemyClips[i-1].x + enemyW;
      } else {
        enemyClips[i].x = 0;
      }
      enemyClips[i].y = 0;
      enemyClips[i].w = enemyW;
      enemyClips[i].h = enemyH;
    }

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
                enemies[i].width = enemyW;
        enemies[i].height = enemyH;
    }

	// TO-DO: Make this less hacky
	player.texture = TextureCache_CreateTexture(ANIM_IMG, game->renderer);

	currentTime = SDL_GetTicks();

	//load sound file
	collideSound = Mix_LoadMUS(COLLIDE_SND);
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

void PlayState::close() {
      Mix_FreeMusic(collideSound);
}

void PlayState::pause() {
  paused = true;
}

void PlayState::resume() {
  paused = false;
}

void PlayState::handleEvents(Game* game) {
  SDL_Event e;
  if (SDL_PollEvent(&e)) {
    switch(e.type) {
    case SDL_QUIT:
      Game_Close(game);
      break;
    case SDL_KEYDOWN:
      switch(e.key.keysym.sym) {
      case SDLK_m:
      case SDLK_q:
	Game_ChangeState(game, MenuState::Instance());
	break;
      case SDLK_p:
	paused = !paused;
      case SDLK_w:
      case SDLK_a:
      case SDLK_s:
      case SDLK_d:
	  keysdown[e.key.keysym.sym] = true;
	  break;
      case SDLK_ESCAPE:
	Game_Close(game);
	break;
      }
      break;
      case SDL_KEYUP:
          switch (e.key.keysym.sym) {
	    case SDLK_w:
	    case SDLK_a:
            case SDLK_s:
	    case SDLK_d:
		keysdown[e.key.keysym.sym] = false;
		break;
	    default:
		break;
          }
    }
  }
}

void PlayState::update(Game* game) {
  if (paused) {
    currentTime = SDL_GetTicks();
  }
 	if (!paused) {
	        lastTime = currentTime;
	        currentTime = SDL_GetTicks();
	        frameTime = currentTime - lastTime;
	        delta = frameTime / ((float)OptimalTime_);
	        if (frameTime < OptimalTime_) {
	            SDL_Delay((OptimalTime_) - frameTime);
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
                enemies[i].undoMove();
                enemies[i].reverseDirection();
            } else {
                for (int j = 0; j < MaxEnemies_; j++) {
                    if (i != j) {
		      if (Collision::collision(enemies[i].position, enemies[i].texture->w,
					       enemies[i].texture->h, enemies[j].position,
					       enemies[j].texture->w,
					       enemies[j].texture->h)) {
                            enemies[i].undoMove();
                            enemies[i].reverseDirection();
                        }
                    }
                }
            }
        }
    }

}

void PlayState::draw(Game* game) {
    uint32 sprite = (currentTime / 100) % 4;
    SDL_Rect * currentClip = &playerClips[sprite];
        SDL_Rect * oppClip = &enemyClips[sprite];

	// Render
	SDL_RenderClear(game->renderer);
	Renderer_RenderCoord(game->renderer, &player.position, player.texture, currentClip);
	for (int i = 0; i < MaxEnemies_; i++) {
		Renderer_RenderCoord(game->renderer, &enemies[i].position, enemies[i].texture, oppClip);
	}
	SDL_RenderPresent(game->renderer);
}
