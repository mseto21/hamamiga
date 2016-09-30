#include "InputComponent.h"
#include "MovementComponent.h"
#include "constants.h"
#include <SDL.h>

InputComponent::InputComponent(MovementComponent* movementComponent) {
	Component::Component();
	this->movementComponent = movementComponent;
}

void InputComponent::Update(float timestep, bool* keysPressed) {
	for (int entityIndex = 0; entityIndex < count; entityIndex++) {
		if (!movementComponent->HasIndex(entityIndex)) {
			continue;
		}
		MovementValues* moveValues = &movementComponent->movementValues[entityArray[entityIndex]];
		this->xvel = 0;
		this->yvel = 0;
		if (keysPressed[SDLK_w]) {
			this->yvel = -Constants::StepSize_;
		}
		if (keysPressed[SDLK_a]) {
			this->xvel = -Constants::StepSize_;
		}
		if (keysPressed[SDLK_s]) {
			this->yvel = Constants::StepSize_;
		}
		if (keysPressed[SDLK_d]) {
			this->xvel = Constants::StepSize_;
		}
	}
}