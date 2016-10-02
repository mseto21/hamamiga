#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "SDL.h"
#include "gamestate.h"

class TitleState : public GameState {
    public:
        TitleState() { }
        void initialize();
        void close();
        void pause();
        void resume();
        void handleEvents(Game* game);
        void update(Game* game);
        void draw(Game* game);
        static TitleState* Instance() {
            return &titleState;
        }
    private:
	static TitleState titleState;

	SDL_Surface* bGround;
	SDL_Surface* fader;
	int alpha;
};

#endif
