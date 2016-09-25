#include "Enemy.h"
#include "constants.h"
#include <cstdlib>    
#include <ctime>      

Enemy::Enemy() : destination(),  maxPosition() {
    srand(time(NULL));
    maxPosition.x = Constants::ScreenWidth_;
    maxPosition.y = Constants::ScreenHeight_;//temporary
    newDestination();
    position.x = 0;
    position.y = 0;//temporary
}

void Enemy::newDestination() {
    destination.x = rand() % maxPosition.x+1;
    destination.y = rand() % maxPosition.y+1;
}

void Enemy::move() {

    //check if destination has been reached
    if (position.x == destination.x && position.y == destination.y) {
        newDestination();
    }

    // move character closer to destination
    if (position.x != destination.x) {
        position.x = position.x < destination.x ? position.x+1 : position.x-1;
    } else if (position.y != destination.y) {
        position.y = position.y < destination.y ? position.y+1 : position.y-1;
    }
}
