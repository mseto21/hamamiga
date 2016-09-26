#include "Player.h"
#include "constants.h"
#include <SDL.h>

//const char* path = "/assets/player.png";

Player::Player() {
	position.x = 0;
	position.y = 0;
  	width = 32; //width of sprite
  	height = 32;
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
				dir = 1;
				break;
			case SDLK_a: //left
				dir = 2;
				break;
			case SDLK_s: //down
				dir = 3;
				break;
			case SDLK_d: //right
				dir = 4;
				break;
		}
	} else {
		dir = 0;
	}
}


void Player::Update(float timestep) {
	switch (this->dir) {
		case 1:
			if ((this->position.y - Constants::STEPSIZE * timestep) >= 0){//checks in bounds
				this->position.y -= Constants::STEPSIZE * timestep;
			}
			break;
		case 2:
			if ((this->position.x - Constants::STEPSIZE * timestep) >= 0){
				this->position.x -= Constants::STEPSIZE * timestep;
			}
			break;
		case 3:
			if ((this->position.y + Constants::STEPSIZE * timestep) <=
			    (Constants::ScreenHeight_ - this->height)){
				this->position.y += Constants::STEPSIZE * timestep;
			}
			break;
		case 4:
			if ((this->position.x + Constants::STEPSIZE * timestep) <=
			    (Constants::ScreenWidth_ - this->width)){
				this->position.x += Constants::STEPSIZE * timestep;
			}
			break;
		default:
			break;
	}
}