#include "Player.hpp"
#include "constants.h"

//Moves the player
void Player_Move(Player * player, SDL_Keycode key){
	switch(key) {
		case SDLK_w: //up
			if ((player.position.y - STEPSIZE) >= 0){
				player.position.y -= STEPSIZE;
			}
		case SDLK_a: //left
			if ((player.position.x - STEPSIZE) >= 0){
				player.position.x -= STEPSIZE;
			}
		case SDLK_s: //down
			if ((player.position.y + STEPSIZE) <=
			    (ScreenHeight_ - player.height)){
				player.position.y += STEPSIZE;
			}
		case SDLK_d: //right
			if ((player.position.x + STEPSIZE) <=
			    (ScreenWidth_ - player.width)){
				player.position.x += STEPSIZE;
			}
	}
}
