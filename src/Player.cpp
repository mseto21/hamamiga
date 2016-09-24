#include "Player.hpp"
#include "constants.h"

//Moves the player
void Player_Move(Player * player, SDL_Keycode key){
	switch(key) {
		case SDLK_w: //up
			if ((player.y - STEPSIZE) >= 0){
				player.y -= STEPSIZE;
			}
		case SDLK_a: //left
			if ((player.x - STEPSIZE) >= 0){
				player.x -= STEPSIZE;
			}
		case SDLK_s: //down
			if ((player.y + STEPSIZE) <= (ScreenHeight_ - PlayerHSize_)){
				player.y += STEPSIZE;
			}
		case SDLK_d: //right
			if ((player.x + STEPSIZE) <= (ScreenWidth_ - PlayerWSize_)){
				player.x += STEPSIZE;
			}
	}
}