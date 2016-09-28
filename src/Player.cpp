#include "Player.h"
#include "constants.h"
#include <SDL.h>

Player::Player() {
	position.x = 0;
	position.y = 0;
	lastPosition = { 0, 0 };
	// width = 67; //width of sprite
	// height = 41;
	vx = 5;
	vy = 5;
	ax = 5;
	ay = 5;
	xvel = 0;
	yvel = 0;
}

//Moves the player
void Player::GetInput(bool KeysDown[]) {
	this->xvel = 0;
	this->yvel = 0;
	if (KeysDown[SDLK_w]) {
		this->yvel = -Constants::StepSize_;
	}
	if (KeysDown[SDLK_a]) {
		this->xvel = -Constants::StepSize_;
	}
	if (KeysDown[SDLK_s]) {
		this->yvel = Constants::StepSize_;
	}
	if (KeysDown[SDLK_d]) {
		this->xvel = Constants::StepSize_;
	}
}

void Player::Update(float timestep) {
	lastPosition = { position.x, position.y };
	this->position.y += (int)(this->yvel * timestep);
	if (this->position.y < 0) {
		this->position.y = 0;
	}
	if (this->position.y + this->texture->h > Constants::ScreenHeight_) {
		this->position.y = Constants::ScreenHeight_ - this->texture->h;
	}

	this->position.x += (int)(this->xvel * timestep);
	if (this->position.x < 0) {
		this->position.x = 0;
	}
	if (this->position.x + this->texture->w > Constants::ScreenWidth_) {
		this->position.x = Constants::ScreenWidth_ - this->texture->w;
	}
}

void Player::UndoMove() {
	position = { lastPosition.x, lastPosition.y };
}







