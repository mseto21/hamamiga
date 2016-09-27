#include "Enemy.h"
#include "constants.h"
#include <cstdlib>    
#include <ctime>     
#include <SDL.h>

Enemy::Enemy(SDL_Rect r, Coord2D pos) : destination(),  maxPosition() {
    srand(time(NULL));
    rect = r;
    maxPosition.x = Constants::ScreenWidth_-rect.w;
    maxPosition.y = Constants::ScreenHeight_-rect.h;
    newDestination();
    position = pos;
}

Enemy::Enemy() {
    srand(time(NULL));
    texture = NULL;
    maxPosition.x = Constants::ScreenWidth_;
    maxPosition.y = Constants::ScreenHeight_;
    newDestination();
    position.x = 0;
    position.y = 0;
}

void Enemy::newDestination() {
    destination.x = rand() % (int)maxPosition.x+1;
    destination.y = rand() % (int)maxPosition.y+1;
}

void Enemy::Update(float timestep) {

    //check if destination has been reached
    if (position.x == destination.x && position.y == destination.y) {
        newDestination();
    }

    // move character closer to destination
    if (position.x != destination.x) {
        position.x = position.x < destination.x ? position.x+(int)(Constants::StepSize_*timestep) : position.x-(int)(Constants::StepSize_*timestep);
    } else if (position.y != destination.y) {
        position.y = position.y < destination.y ? position.y+(int)(Constants::StepSize_*timestep) : position.y-(int)(Constants::StepSize_*timestep);
    }
}
