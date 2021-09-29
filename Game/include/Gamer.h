#pragma once

#include <vector>
#include <memory>
#include "GameObject.h"
#include "ViewableContainer.h"

class Gamer: public std::enable_shared_from_this<Gamer>, public ViewableContainer<GameObject, GameObject::Id> {
protected:
    double money;

public:
    // TODO: should only affect gamer's inventories and cash, not world, world will do this itself
    virtual void buyItem(std::unique_ptr<GameObject> item);
    virtual std::unique_ptr<GameObject> sellItem(GameObject::Id itemId, GameObject::Cost newCost);
    std::unique_ptr<GameObject> sellItem(GameObject::Id itemId);
    double getBalance() { return money; }
};