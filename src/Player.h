#include "Character.h"
#include "types.h"
#include <SDL.h>

struct Texture;

class Player : public Character
{
	public:
		void GetInput(SDL_Event* event);
		void Update(float timestep);
		Texture* texture;
		SDL_Rect rect;
		Player(SDL_Rect r);
    	//Player();
	private:
		uint8 dir;
};
