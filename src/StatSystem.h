struct ComponentBag;
struct HealthComponent;

struct StatSystem {
	HealthComponent* healthComponent;
};

void StatSystem_Initialize(StatSystem* statSystem, ComponentBag* cBag);
void StatSystem_Update(StatSystem* statSystem, float delta);