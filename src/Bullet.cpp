#include "Bullet.h"

Bullet::Bullet() {
	alive = false;
	position.x = 0;
  position.y = 0;
}

void Bullet::initialize(Rectangle playerPos, float frameTime){
	this->bullet.x = playerPos.x + 60 ;
	this->bullet.y = playerPos.y + 30 ;
	this->currentFrameTime = 0.f;
	this->frameTime =  frameTime;
}

void Bullet::update(uint32 delta){
	if (this->alive == true){
		this->bullet.x += this->velocity.x * delta;
		this->bullet.y += this->velocity.y * delta;
	}
}

void Bullet::load(Texture* texture){
	this->texture = texture;
}

void Bullet::display(){
	this->bullet.x += 5;
	//bullet.bullet.w = bullet.w;
	//bullet.bullet.h = bullet.h;
	//SDL_FillRect(screen , &bullet.bullet , 0xFFFF66);
}