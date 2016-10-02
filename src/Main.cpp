#include "Game.h"
int main(void) {
	Game game;
	if (!Game_Initialize(&game)) {
		return 1;
	}
	game.changeState(TitleState::Instance());
	Game_RunLoop(&game);
	Game_Close(&game);
	return 0;
}
