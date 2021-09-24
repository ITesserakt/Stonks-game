#pragma once

#include <vector>
#include <memory>
#include "GameObject.h"

class Gamer: public std::enable_shared_from_this<Gamer> {
protected:
    std::vector<std::unique_ptr<GameObject>> inventory;
    double money;

public:
    // TODO: should only affect gamer's inventories and cash, not world, world will do this itself
    virtual void buyItem(std::unique_ptr<GameObject> item);
    virtual std::unique_ptr<GameObject> sellItem(GameObject::Id itemId, GameObject::Cost newCost);

    std::unique_ptr<GameObject> takeItem(GameObject::Id itemId);
    const GameObject& viewItem(GameObject::Id itemId) const;
};