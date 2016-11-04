#include "Bullet.h"
#include "Constants.h"

Bullet::Bullet() {
	alive = true;
	position.x = 0;
  position.y = 0;

}


void Bullet::update(uint32 delta){
	if (this->position.x  < Constants::ScreenWidth_){
		//this->position.x += .05;
	} else {
		alive = false; //kill this bullet
	}
	//age++;
		//this->bullet.y += this->velocity.y * delta;
	//}
}
