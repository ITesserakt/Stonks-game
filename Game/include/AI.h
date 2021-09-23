#include "Gamer.h"
#include "World.h"

class AI : public Gamer {
private:
    World world;

public:
    GameObject::Id predict();
};