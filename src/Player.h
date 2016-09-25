#include "Character.h"
#include "types.h"

struct Texture;

class Player : public Character
{
	public:
		Player();
		void GetInput(SDL_Event* event);
		void Update(float timestep);
		Texture* texture;
	private:
		uint8 dir;
};
