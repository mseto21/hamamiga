#include "Character.h"
#include "types.h"
#include <SDL.h>

struct Texture;

class Player : public Character
{
	public:
		Player();
		void GetInput(bool KeysDown[]);
		void Update(float timestep);
		void UndoMove();
		Texture* texture;
		SDL_Rect rect;
		Player(SDL_Rect r);
    	//Player();
	private:
		float xvel;
		float yvel;
};
