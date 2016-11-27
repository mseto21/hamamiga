struct Zone;
struct ComponentBag;
struct SDL_Renderer;
struct ZoneIntroState;

bool FileLoader_Load(Zone* zone, const char* path, ComponentBag* cBag, SDL_Renderer* renderer, ZoneIntroState* zoneIntroState);
bool FileLoader_LoadScores(const char* path, int[] stats, SDL_Renderer* renderer);