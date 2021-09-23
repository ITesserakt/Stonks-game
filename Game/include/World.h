#pragma once

#include <vector>
#include "Gamer.h"
#include "ObjectFactory.h"

class World {
    friend class game_logic_profitness_Test;

private:
    std::vector<std::unique_ptr<Gamer>> players;
    std::vector<std::unique_ptr<GameObject>> slots;
    unsigned int availableSlots = 10;
    ObjectFactory factory;

public:
    explicit World(ObjectFactory &&factory);

    double getProfitness(const GameObject::Id& itemId);

    std::vector<GameObject::Id> getSlots();
};