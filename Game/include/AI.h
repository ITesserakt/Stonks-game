#pragma once

#include "Gamer.h"
#include "World.h"

// This man buys products and sell them again
class AI : public Gamer {
private:
    World& world;
    bool debugFlag;

public:
    AI(World &state, bool debug, unsigned int maxSlots);
    virtual ~AI() = default;

    GameObject::Id predictToBuy();

    GameObject::Id predictToSell();

    double getProfitness(GameObject::Id itemId);

    void startTrading();
};