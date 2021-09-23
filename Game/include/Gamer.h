#pragma once

#include <vector>
#include <memory>
#include "GameObject.h"

class Gamer {
private:
    std::vector<std::unique_ptr<GameObject>> inventory;
    double money;

public:
    // TODO: should only affect gamer's inventories and cash, not world, world will do this itself
    virtual void buyItem(std::unique_ptr<GameObject> item) = 0;
    virtual std::unique_ptr<GameObject> sellItem() = 0;
};