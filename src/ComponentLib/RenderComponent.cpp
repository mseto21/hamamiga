#include "RenderComponent.h"
#include "RectangleComponent.h"

RenderComponent::RenderComponent(RectangleComponent* rectangleComponent) {
	Component::Component();
	this->rectangleComponent = rectangleComponent;
}

void RenderComponent::SetOn(uint32 eid, Texture* texture) {
	Component::SetOn(eid);
	textures[eid] = texture;
}