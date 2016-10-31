struct ComponentBag;
struct AliveComponent;

struct KillSystem {
	ComponentBag* cBag;
	AliveComponent* aliveComponent;
};

void KillSystem_Initialize(KillSystem* killSystem, ComponentBag* cBag);
void KillSystem_Update(KillSystem* killSystem);
void KillSystem_Free(KillSystem* killSystem);