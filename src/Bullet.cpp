#include "Bullet.h"

Bullet::Bullet() {
	alive = false;
	//width = 5;
	//height = 5;
	x = 0;
  y = 0;
  w = 10;
  h = 10;
}

void Bullet::initialize(Rectangle playerPos){
alive = true;
 this->x = playerPos.x + 60 ;
 this->y = playerPos.y + 30 ;
}

void Bullet::load(Texture* texture){
	this->texture = texture;
}

void Bullet::display(){
	this->x += 5;
	//bullet.bullet.w = bullet.w;
	//bullet.bullet.h = bullet.h;
	//SDL_FillRect(screen , &bullet.bullet , 0xFFFF66);
}