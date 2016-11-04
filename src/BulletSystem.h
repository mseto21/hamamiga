#pragma once

//Forward Declarations
struct BulletComponent;
struct RectangleComponent;
struct ComponentBag;

struct BulletSystem {
  //MovementComponent* movementComponent;
  RectangleComponent* rectangleComponent;
  BulletComponent* bulletComponent;
};

void BulletSystem_Initialize(BulletSystem* bulletSystem, ComponentBag* cBag);
void BulletSystem_Update(BulletSystem* bulletSystem);
void BulletSystem_Free(BulletSystem* bulletSystem);