#include "Types.h"
struct Game;

void LoadIntroStateAssets(Game* game);
void LoadTitleStateAssets(Game* game);
void LoadHighScoreStateAssets(Game* game);
void LoadLevelStatAssets(Game* game);
void LoadLevelSelectAssets(Game* game);
void LoadOptionStateAssets(Game* game);
void LoadZoneIntroAssets(Game* game, String128 name);
bool LoadPlayStateAssets(Game* game, int chapter);
void FreePlay(Game* game);