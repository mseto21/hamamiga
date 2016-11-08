#include "ComponentBag.h"
#include "RectangleComponent.h"
#include "MovementComponent.h"
#include "TextureComponent.h"
#include "InputComponent.h"
#include "AnimationComponent.h"
#include "BulletComponent.h"
#include "PhysicsComponent.h"
#include "HealthComponent.h"
#include "CameraComponent.h"
#include "HatComponent.h"
#include "AIComponent.h"
#include "AliveComponent.h"
#include "GoalComponent.h"
#include "InteractableComponent.h"
#include <iostream>


void ComponentBag_Malloc(ComponentBag* bag) {
	bag->rectangleComponent 	= (RectangleComponent*)malloc(sizeof(*bag->rectangleComponent));
	bag->movementComponent 		= (MovementComponent*)malloc(sizeof(*bag->movementComponent));
	bag->textureComponent 		= (TextureComponent*)malloc(sizeof(*bag->textureComponent));
	bag->inputComponent 		= (InputComponent*)malloc(sizeof(*bag->inputComponent));
	bag->animationComponent 	= (AnimationComponent*)malloc(sizeof(*bag->animationComponent));
	bag->bulletComponent 	= (BulletComponent*)malloc(sizeof(*bag->bulletComponent));
	bag->physicsComponent 		= (PhysicsComponent*)malloc(sizeof(*bag->physicsComponent));
	bag->healthComponent 		= (HealthComponent*)malloc(sizeof(*bag->healthComponent));
	bag->cameraComponent 		= (CameraComponent*)malloc(sizeof(*bag->cameraComponent));
	bag->hatComponent 			= (HatComponent*)malloc(sizeof(*bag->hatComponent));
	bag->aiComponent 			= (AIComponent*)malloc(sizeof(*bag->aiComponent));
	bag->aliveComponent 		= (AliveComponent*)malloc(sizeof(*bag->aliveComponent));
	bag->goalComponent 			= (GoalComponent*)malloc(sizeof(*bag->goalComponent));
	bag->interactableComponent 	= (InteractableComponent*)malloc(sizeof(*bag->interactableComponent));
	ComponentBag_Reset(bag);
}

void ComponentBag_Reset(ComponentBag* bag) {
	Component_Initialize(bag->rectangleComponent);
	Component_Initialize(bag->movementComponent);
	Component_Initialize(bag->physicsComponent);
	Component_Initialize(bag->textureComponent);
	Component_Initialize(bag->inputComponent);
	Component_Initialize(bag->animationComponent);
	Component_Initialize(bag->bulletComponent);
	Component_Initialize(bag->healthComponent);
	Component_Initialize(bag->cameraComponent);
	Component_Initialize(bag->hatComponent);
	Component_Initialize(bag->aiComponent);
	Component_Initialize(bag->aliveComponent);
	Component_Initialize(bag->goalComponent);
	InteractableComponent_Initialize(bag->interactableComponent);
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
	if (!bag->bulletComponent) {
		std::cout << "Error: Uninitialized bulletComponent" << std::endl;
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
	if (!bag->aliveComponent) {
	  std::cout << "Error: Uninitialized aliveComponent" << std::endl;
	}
	if (!bag->goalComponent) {
	  std::cout << "Error: Uninitialized aliveComponent" << std::endl;
	}
	if (!bag->interactableComponent) {
	  std::cout << "Error: Uninitialized aliveComponent" << std::endl;
	}
}

void ComponentBag_Free(ComponentBag* bag) {
	free(bag->rectangleComponent);
	bag->rectangleComponent = nullptr;
	free(bag->movementComponent);
	bag->movementComponent = nullptr;
	free(bag->textureComponent);
	bag->textureComponent = nullptr;
	free(bag->inputComponent);
	bag->inputComponent = nullptr;
	free(bag->animationComponent);
	bag->animationComponent = nullptr;
	free(bag->bulletComponent);
	bag->bulletComponent = nullptr;
	free(bag->physicsComponent);
	bag->physicsComponent = nullptr;
	free(bag->healthComponent);
	bag->healthComponent = nullptr;
	free(bag->cameraComponent);
	bag->cameraComponent = nullptr;
	free(bag->hatComponent);
	bag->hatComponent = nullptr;
	free(bag->aiComponent);
	bag->aiComponent = nullptr;
	free(bag->aliveComponent);
	bag->aliveComponent = nullptr;
	free(bag->goalComponent);
	bag->goalComponent = nullptr;
	// TO-DO: Make this less unfortunate (probably never).
	InteractableComponent_Free(bag->interactableComponent);
	free(bag->interactableComponent);
	bag->interactableComponent = nullptr;
}

void Component_DisableEntity(ComponentBag* bag, uint32 eid) {
	if (Component_HasIndex(bag->rectangleComponent, eid)) {
		Component_Disable(bag->rectangleComponent, eid);
	}
	if (Component_HasIndex(bag->movementComponent, eid)) {
		Component_Disable(bag->movementComponent, eid);
	}
	if (Component_HasIndex(bag->physicsComponent, eid)) {
		Component_Disable(bag->physicsComponent, eid);
	}	
	if (Component_HasIndex(bag->textureComponent, eid)) {
		Component_Disable(bag->textureComponent, eid);
	}
	if (Component_HasIndex(bag->inputComponent, eid)) {
		Component_Disable(bag->inputComponent, eid);
	}
	if (Component_HasIndex(bag->animationComponent, eid)) {
		Component_Disable(bag->animationComponent, eid);
	}
	if (Component_HasIndex(bag->bulletComponent, eid)) {
		Component_Disable(bag->bulletComponent, eid);
	}
	if (Component_HasIndex(bag->healthComponent, eid)) {
		Component_Disable(bag->healthComponent, eid);
		bag->healthComponent->health[eid] = 0;
	}
	if (Component_HasIndex(bag->cameraComponent, eid)) {
		Component_Disable(bag->cameraComponent, eid);
	}
	if (Component_HasIndex(bag->hatComponent, eid)) {
		Component_Disable(bag->hatComponent, eid);
	}
	if (Component_HasIndex(bag->aiComponent, eid)) {
		Component_Disable(bag->aiComponent, eid);
	}
	if (Component_HasIndex(bag->aliveComponent, eid)) {
		Component_Disable(bag->aliveComponent, eid);
	}
	if (Component_HasIndex(bag->goalComponent, eid)) {
		Component_Disable(bag->goalComponent, eid);
	}
	if (Component_HasIndex(bag->interactableComponent, eid)) {
		Component_Disable(bag->interactableComponent, eid);
	}
}

void ComponentBag_ForceRemove(ComponentBag* bag, uint32 eid) {
	if (Component_HasIndex(bag->rectangleComponent, eid)) {
		Component_ForceRemove(bag->rectangleComponent, eid);
	}
	if (Component_HasIndex(bag->movementComponent, eid)) {
		Component_ForceRemove(bag->movementComponent, eid);
	}
	if (Component_HasIndex(bag->physicsComponent, eid)) {
		Component_ForceRemove(bag->physicsComponent, eid);
	}	
	if (Component_HasIndex(bag->textureComponent, eid)) {
		Component_ForceRemove(bag->textureComponent, eid);
	}
	if (Component_HasIndex(bag->inputComponent, eid)) {
		Component_ForceRemove(bag->inputComponent, eid);
	}
	if (Component_HasIndex(bag->animationComponent, eid)) {
		Component_ForceRemove(bag->animationComponent, eid);
	}
	if (Component_HasIndex(bag->bulletComponent, eid)) {
		Component_ForceRemove(bag->bulletComponent, eid);
	}
	if (Component_HasIndex(bag->healthComponent, eid)) {
		Component_ForceRemove(bag->healthComponent, eid);
		bag->healthComponent->health[eid] = 0;
	}
	if (Component_HasIndex(bag->cameraComponent, eid)) {
		Component_ForceRemove(bag->cameraComponent, eid);
	}
	if (Component_HasIndex(bag->hatComponent, eid)) {
		Component_ForceRemove(bag->hatComponent, eid);
	}
	if (Component_HasIndex(bag->aiComponent, eid)) {
		Component_ForceRemove(bag->aiComponent, eid);
	}
	if (Component_HasIndex(bag->aliveComponent, eid)) {
		Component_ForceRemove(bag->aliveComponent, eid);
	}
	if (Component_HasIndex(bag->goalComponent, eid)) {
		Component_ForceRemove(bag->goalComponent, eid);
	}
	if (Component_HasIndex(bag->interactableComponent, eid)) {
		Component_ForceRemove(bag->interactableComponent, eid);
	}
}


