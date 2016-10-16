#include "Component.h"
#include <SDL.h>

struct CameraComponent : public Component {
	SDL_Rect camera;
};

void CameraComponent_Add(CameraComponent* cameraComponent, uint32 eid);