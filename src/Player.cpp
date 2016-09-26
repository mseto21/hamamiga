#include "Player.h"
#include "constants.h"
#include <SDL.h>

//const char* path = "/assets/player.png";

Player::Player(SDL_Rect playerRect){
		position.x = playerRect.x;
		position.y = playerRect.y;
  	width =  playerRect.w; //width of sprite
  	height = playerRect.h;
  	vx = 5;
  	vy = 5;
  	ax = 5;
  	ay = 5;
  	dir = 0;
}

//Moves the player
void Player::GetInput(SDL_Event* event) {
	if (event->type == SDL_KEYDOWN) {
		SDL_Keycode key = event->key.keysym.sym;
		switch(key) {
			case SDLK_w: //up
				this->dir = 1;
				break;
			case SDLK_a: //left
				this->dir = 2;
				break;
			case SDLK_s: //down
				this->dir = 3;
				break;
			case SDLK_d: //right
				this->dir = 4;
				break;
		}
	}
}


void Player::Update(float timestep) {
	switch (this->dir) {
		case 1:
			if ((this->position.y - Constants::STEPSIZE) >= 0){//checks in bounds
				this->position.y -= Constants::STEPSIZE;// * timestep;
			}
			break;
		case 2:
			if ((this->position.x - Constants::STEPSIZE) >= 0){
				this->position.x -= Constants::STEPSIZE;// * timestep;
			}
			break;
		case 3:
			if ((this->position.y + Constants::STEPSIZE) <=
			    (Constants::ScreenHeight_ - this->height)){
				this->position.y += Constants::STEPSIZE;// * timestep;
			}
			break;
		case 4:
			if ((this->position.x + Constants::STEPSIZE) <=
			    (Constants::ScreenWidth_ - this->width)){
				this->position.x += Constants::STEPSIZE;// * timestep;
			}
			break;
		default:
			break;
	}
	this->dir = 0;
}