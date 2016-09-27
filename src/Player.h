#include "Character.h"
#include "types.h"

struct Texture;

class Player : public Character
{
	public:
		Player();
		void GetInput(bool* KeysDown);
		void Update(float timestep);
		Texture* texture;
	private:
		float xvel;
		float yvel;
};
