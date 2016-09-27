#include "Player.h"
#include "constants.h"

Player::Player() {
	rect = {0, 0, 68, 42};
	vx = 5;
	vy = 5;
	ax = 5;
	ay = 5;
	xvel = 0;
	yvel = 0;
}

//Moves the player
void Player::GetInput(bool* KeysDown) {
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
	this->rect.y += (int)(this->yvel * timestep);
	if (this->rect.y < 0) {
		this->rect.y = 0;
	}
	if (this->rect.y + this->texture->h > Constants::ScreenHeight_) {
		this->rect.y = Constants::ScreenHeight_ - this->texture->h;
	}

	this->rect.x += (int)(this->xvel * timestep);
	if (this->rect.x < 0) {
		this->rect.x = 0;
	}
	if (this->rect.x + this->texture->w > Constants::ScreenWidth_) {
		this->rect.x = Constants::ScreenWidth_ - this->texture->w;
	}
}

void Player::Collide(Character characters[]) {
	return;
}





