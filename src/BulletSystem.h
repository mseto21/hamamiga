#pragma once
#include "Constants.h"
#include "Types.h"

//Forward Declarations
struct PhysicsComponent;
struct RectangleComponent;
struct BulletComponent;
struct AliveComponent;
struct TextureComponent;
struct ComponentBag;

struct BulletSystem {
  PhysicsComponent* physicsComponent;
  RectangleComponent* rectangleComponent;
  BulletComponent* bulletComponent;
  AliveComponent* aliveComponent;
  TextureComponent* textureComponent;

 // uint32 eidbullets[Constants::MaxBullets_];//array of bullet entities
};

void BulletSystem_Initialize(BulletSystem* bulletSystem, ComponentBag* cBag);
void BulletSystem_Update(BulletSystem* bulletSystem);
void BulletSystem_Free(BulletSystem* bulletSystem);