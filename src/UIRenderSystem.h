#pragma once
#include "Types.h"
struct Game;

void UIRenderSystem_Render(int gameState, Game* game, uint32 elapsed, bool* keysdown, bool* keysup);