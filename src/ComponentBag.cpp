#include "ComponentBag.h"
#include "RectangleComponent.h"
#include "MovementComponent.h"
#include "TextureComponent.h"
#include "InputComponent.h"
#include "AnimationComponent.h"
#include "PhysicsComponent.h"
#include "HealthComponent.h"
#include "CameraComponent.h"
#include "HatComponent.h"
#include "AIComponent.h"
#include <iostream>

void ComponentBag_Malloc(ComponentBag* bag) {
	bag->rectangleComponent 	= (RectangleComponent*)malloc(sizeof(*bag->rectangleComponent));
	bag->movementComponent 		= (MovementComponent*)malloc(sizeof(*bag->movementComponent));
	bag->textureComponent 		= (TextureComponent*)malloc(sizeof(*bag->textureComponent));
	bag->inputComponent 		= (InputComponent*)malloc(sizeof(*bag->inputComponent));
	bag->animationComponent 	= (AnimationComponent*)malloc(sizeof(*bag->animationComponent));
	bag->physicsComponent 		= (PhysicsComponent*)malloc(sizeof(*bag->physicsComponent));
	bag->healthComponent 		= (HealthComponent*)malloc(sizeof(*bag->healthComponent));
	bag->cameraComponent 		= (CameraComponent*)malloc(sizeof(*bag->cameraComponent));
	bag->hatComponent 			= (HatComponent*)malloc(sizeof(*bag->hatComponent));
	bag->aiComponent 			= (AIComponent*)malloc(sizeof(*bag->aiComponent));
	ComponentBag_Reset(bag);
	bag->freed = false;
}

void ComponentBag_Reset(ComponentBag* bag) {
	Component_Initialize(bag->rectangleComponent);
	Component_Initialize(bag->movementComponent);
	Component_Initialize(bag->physicsComponent);
	Component_Initialize(bag->textureComponent);
	Component_Initialize(bag->inputComponent);
	Component_Initialize(bag->animationComponent);
	Component_Initialize(bag->healthComponent);
	Component_Initialize(bag->cameraComponent);
	Component_Initialize(bag->hatComponent);
	Component_Initialize(bag->aiComponent);
}

void ComponentBag_Check(ComponentBag* bag) {
	if (!bag->rectangleComponent) {
		std::cout << "Error: Uninitialized rectangleComponent" << std::endl;
	}
	if (!bag->movementComponent) {
		std::cout << "Error: Uninitialized movementComponent" << std::endl;
	}
	if (!bag->textureComponent) {
		std::cout << "Error: Uninitialized textureComponent" << std::endl;
	}
	if (!bag->inputComponent) {
		std::cout << "Error: Uninitialized inputComponent" << std::endl;
	}
	if (!bag->animationComponent) {
		std::cout << "Error: Uninitialized animationComponent" << std::endl;
	}
	if (!bag->physicsComponent) {
		std::cout << "Error: Uninitialized physicsComponent" << std::endl;
	}
	if (!bag->healthComponent) {
		std::cout << "Error: Uninitialized healthComponent" << std::endl;
	}
	if (!bag->cameraComponent) {
		std::cout << "Error: Uninitialized cameraComponent" << std::endl;
	}
	if (!bag->hatComponent) {
	  std::cout << "Error: Uninitialized hatComponent" << std::endl;
	}
	if (!bag->aiComponent) {
	  std::cout << "Error: Uninitialized aiComponent" << std::endl;
	}
}

void ComponentBag_Free(ComponentBag* bag) {
	free(bag->rectangleComponent);
	free(bag->movementComponent);
	free(bag->textureComponent);
	free(bag->inputComponent);
	free(bag->animationComponent);
	free(bag->physicsComponent);
	free(bag->healthComponent);
	free(bag->cameraComponent);
	free(bag->hatComponent);
	free(bag->aiComponent);
       	bag->freed = true;
}
