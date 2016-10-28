struct Zone;
struct ComponentBag;
struct SDL_Renderer;

bool FileLoader_Load(Zone* zone, const char* path, ComponentBag* cBag, SDL_Renderer* renderer);