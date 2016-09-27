#include "Character.h"

class Player : public Character
{
	public:
		Player();
		void GetInput(bool* KeysDown);
		void Update(float timestep);
	private:
		float xvel;
		float yvel;
};
