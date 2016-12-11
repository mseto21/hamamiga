#define MaxHats_ 16

enum ScoreType{
  Hats_,
  Coins_,
  Deaths_,
  Fallen_,
  Demons_,
  DemonBats_,
  Muffins_,
  Oranges_,
  Apples_,
  NumScoreTypes_,
};

extern const char* scoreType[NumScoreTypes_]; //Description displayed
extern int numPossibleScores[NumScoreTypes_];
extern int hatsCollected[16]; //Max hats for now
extern int scores[NumScoreTypes_];
extern int numDisplay;

//Goal Data Storage
void Scores_Update(const char* path, const char* type, const char* value);
void LevelScore_Reset();
void CoinScore_Update();
void Scoreboard_Update();
int totalFromFile(const char * path, const char * type);