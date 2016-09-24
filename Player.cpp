#include "Player.hpp"
#include "GameConstants.h"

//Moves the player
void Player_Move(Player * player, SDL_Keycode key){
	switch(key) {
		case SDLK_w: //up
			player.y -= STEPSIZE;
		case SDLK_a: //left
			player.x -= STEPSIZE;
		case SDLK_s: //down
			player.y += STEPSIZE;
		case SDLK_d: //right
			player.x += STEPSIZE;
	}
}