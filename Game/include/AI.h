#pragma once

#include "Gamer.h"
#include "World.h"

// This man buys products and sell them again
class AI : public Gamer {
private:
    const World& world;

public:
    AI(const World &world, unsigned int maxSlots);
    virtual ~AI() = default;

    GameObject::Id predictToBuy();

    GameObject::Id predictToSell();

    double getProfitness(GameObject::Id itemId);
};