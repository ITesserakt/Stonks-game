#pragma once

#include "Gamer.h"
#include "World.h"
#include <thread>

// This man buys products and sell them again
class AI : public Gamer {
private:
    World &world;
    bool debugFlag;

    JSONCONS_TYPE_TRAITS_FRIEND

    AI(World &world, bool debugFlag, double money, unsigned int availableSlots, std::map<GameObject::Id, std::unique_ptr<GameObject>> container);

public:
    AI(World &state, bool debug, unsigned int maxSlots);

    static AI fromJson(World &world, const jsoncons::json &json);

    void writeToJson(jsoncons::json &json);

    GameObject::Id predictToBuy() const;

    GameObject::Id predictToSell() const;

    double getProfitness(GameObject::Id itemId) const;

    std::thread startTrading(const bool &running);
};
