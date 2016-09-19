#include "Game.hpp"

int main(void) {
	Game game;
	if (!Game_Initialize(&game)) {
		return 1;
	}

	Game_RunLoop(&game);
	Game_Close(&game);
	return 0;
}