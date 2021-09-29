#pragma once

#include "Gamer.h"
#include "World.h"

// This man buys products and sell them again
class AI : public Gamer {
private:
    const World& world;

public:
    explicit AI(const World &world);

    GameObject::Id predict();

    void buyItem(std::unique_ptr<GameObject> item) override;

    std::unique_ptr<GameObject> sellItem(GameObject::Id itemId, GameObject::Cost newCost) override;
};