#pragma once

#include "Gamer.h"
#include "World.h"
#include <thread>

// This man buys products and sell them again
class AI : public Gamer {
private:
    World &world;
    bool debugFlag;

public:
    AI(World &state, bool debug, unsigned int maxSlots);

    GameObject::Id predictToBuy() const;

    GameObject::Id predictToSell() const;

    double getProfitness(GameObject::Id itemId) const;

    std::thread startTrading(const bool &running);
};