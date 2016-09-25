#include "Character.h"
#include <SDL.h>

class Player : public Character
{
	public:
		Player();
		void move(SDL_Keycode key);
};
