#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "SDL.h"
#include "GameState.h"

struct Texture;

class TitleState : public GameState {
    public:
        TitleState() { }
        void initialize(Game* game);
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

	Texture* bGround;
	Texture* fader;
	int alpha;
	int count;
};

#endif
