#include "Types.h"
struct Game;

void LoadIntroStateAssets(Game* game);
void LoadTitleStateAssets(Game* game);
void LoadHighScoreStateAssets(Game* game);
void LoadZoneIntroAssets(Game* game, String128 name);
bool LoadPlayStateAssets(Game* game);