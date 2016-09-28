#include "Enemy.h"
#include "constants.h"
#include <cstdlib>    
#include <ctime>     
#include <SDL.h>

Enemy::Enemy(Coord2D pos) : destination(),  maxPosition() {
    srand(time(NULL));
    maxPosition.x = Constants::ScreenWidth_;
    maxPosition.y = Constants::ScreenHeight_;
    newDestination();
    position.x = pos.x;
    position.y = pos.y;
    lastPosition.x = pos.x;
    lastPosition.y = pos.y;
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

void Enemy::update(float timestep) {
    //check if destination has been reached
  if (position.x <= destination.x + (int)(Constants::StepSize_*timestep) && position.x >= destination.x - (int)(Constants::StepSize_*timestep)
      && position.y <= destination.y + (int)(Constants::StepSize_*timestep) && position.y >= destination.y - (int)(Constants::StepSize_*timestep)) {
        newDestination();
    }

    // move character closer to destination
    if (position.x >= destination.x + 1 || position.x <= destination.x - (int)(Constants::StepSize_*timestep)) {
        lastPosition.x = position.x;
        position.x = position.x < destination.x ? position.x+(int)(Constants::StepSize_*timestep) : position.x-(int)(Constants::StepSize_*timestep);
    } else if (position.y >= destination.y + (int)(Constants::StepSize_*timestep) || position.y <= destination.y - (int)(Constants::StepSize_*timestep)) {
        lastPosition.y = position.y;
        position.y = position.y < destination.y ? position.y+(int)(Constants::StepSize_*timestep) : position.y-(int)(Constants::StepSize_*timestep);
    }
}

void Enemy::undoMove() {
    position = { lastPosition.x, lastPosition.y };
}

void Enemy::reverseDirection() {
    newDestination();
}

void Enemy::resetMaxPosition() {
    if (texture != NULL) {
        maxPosition.x = Constants::ScreenWidth_ - texture->w;
        maxPosition.y = Constants::ScreenHeight_ - texture->h;
    }
}


