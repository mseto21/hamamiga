#include "Bullet.h"

Bullet::Bullet() {
	alive = false;
	position.x = 0;
  position.y = 0;
}

void Bullet::initialize(Rectangle playerPos){
 this->bullet.x = playerPos.x + 60 ;
 this->bullet.y = playerPos.y + 30 ;
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