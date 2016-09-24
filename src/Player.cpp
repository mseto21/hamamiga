#include "Player.hpp"
#include "constants.h"

//Moves the player
void Player_Move(Player * player, SDL_Keycode key){
	switch(key) {
		case SDLK_w: //up
			if ((player.y - STEPSIZE) > ScreenHeight_){
				player.y -= STEPSIZE;
			}
		case SDLK_a: //left
			if ((player.x - STEPSIZE) > ScreenWidth_){
				player.x -= STEPSIZE;
			}
		case SDLK_s: //down
			if ((player.y + STEPSIZE) > ScreenHeight_){
				player.y += STEPSIZE;
			}
		case SDLK_d: //right
			if ((player.x + STEPSIZE) > ScreenWidth_){
				player.x += STEPSIZE;
			}
	}
}