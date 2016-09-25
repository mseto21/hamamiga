#include "Player.h"
#include "constants.h"

//Moves the player
void Player::move(SDL_Keycode key) {
	switch(key) {
		case SDLK_w: //up
			if ((this->position.y - Constants::STEPSIZE) >= 0){//checks in bounds
				this->position.y -= Constants::STEPSIZE;
			}
		case SDLK_a: //left
			if ((this->position.x - Constants::STEPSIZE) >= 0){
				this->position.x -= Constants::STEPSIZE;
			}
		case SDLK_s: //down
			if ((this->position.y + Constants::STEPSIZE) <=
			    (Constants::ScreenHeight_ - this->height)){
				this->position.y += Constants::STEPSIZE;
			}
		case SDLK_d: //right
			if ((this->position.x + Constants::STEPSIZE) <=
			    (Constants::ScreenWidth_ - this->width)){
				this->position.x += Constants::STEPSIZE;
			}
	}
}
