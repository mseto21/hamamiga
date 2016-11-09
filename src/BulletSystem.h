#pragma once
#include "Constants.h"
#include "Types.h"

//Forward Declarations
struct PhysicsComponent;
struct RectangleComponent;
struct BulletComponent;
struct AliveComponent;
struct TextureComponent;
struct MovementComponent;
struct ComponentBag;

struct BulletSystem {
  PhysicsComponent* physicsComponent;
  RectangleComponent* rectangleComponent;
  BulletComponent* bulletComponent;
  AliveComponent* aliveComponent;
  TextureComponent* textureComponent;
  MovementComponent* movementComponent;
  ComponentBag* cBag;
};

void BulletSystem_Initialize(BulletSystem* bulletSystem, ComponentBag* cBag);
void BulletSystem_Update(BulletSystem* bulletSystem, uint32 elapsed);
void BulletSystem_Free(BulletSystem* bulletSystem);