/* 
Relies on: RectangleComponent.
Usage: The renderer component doesn't actually run any code. All of its
rendering is handled by the rendering sytem itself.
*/
#include "Component.h"

// Forward Declarations
struct Texture;
class RectangleComponent;

class RenderComponent : public Component {
	RenderComponent(RectangleComponent* rectangleComponent);
	void SetOn(uint32 eid, Texture* texture);
private:
	Texture* textures[Constants::MaxEntities_];
	RectangleComponent* rectangleComponent;
}