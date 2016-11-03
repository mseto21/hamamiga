#include "Bullet.h"
#include "Constants.h"

Bullet::Bullet() {
	alive = false;
	//init = false;
	position.x = 0;
  position.y = 0;
  //currentFrameTime = 0.f;
  //frameTime = 0.f;
  //age = 0;
}

void Bullet::initialize(Rectangle playerPos, float frameTime){
	//this->init = true;
	this->rect.x = playerPos.x + 100 ;
	this->rect.y = playerPos.y;
	//this->currentFrameTime = 0.f;
	//this->frameTime =  frameTime;
}

void Bullet::update(uint32 delta){
	//if (this->alive == true){
	if (this->rect.x  < Constants::ScreenWidth_){
		this->rect.x += .05;
	}
	//age++;
		//this->bullet.y += this->velocity.y * delta;
	//}
}

void Bullet::load(Texture* texture){
	//this->texture = texture;
}

void Bullet::display(){
	this->rect.x += 5;
	//bullet.bullet.w = bullet.w;
	//bullet.bullet.h = bullet.h;
	//SDL_FillRect(screen , &bullet.bullet , 0xFFFF66);
}