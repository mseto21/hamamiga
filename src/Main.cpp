#include "Game.h"
#include "TitleState.h"
int main(void) {
	Game game;
	if (!Game_Initialize(&game)) {
		return 1;
	}
	Game_ChangeState(&game, TitleState::Instance());
	Game_RunLoop(&game);
	Game_Close(&game);
	return 0;
}
