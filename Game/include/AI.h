#include "Gamer.h"
#include "World.h"

// This man buys products and sell them again
class AI : public Gamer {
private:
    World world;

public:
    GameObject::Id predict();
};