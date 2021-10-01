#pragma once

#include <vector>
#include <memory>
#include "GameObject.h"
#include "ViewableContainer.h"
#include <atomic>

class Gamer: public std::enable_shared_from_this<Gamer>, public ViewableContainer<GameObject, GameObject::Id> {
protected:
    double money;
    std::atomic<unsigned int> availableSlots;

public:
    virtual void buyItem(std::unique_ptr<GameObject> item);
    virtual std::unique_ptr<GameObject> sellItem(GameObject::Id itemId, GameObject::Cost newCost);
    std::unique_ptr<GameObject> sellItem(GameObject::Id itemId);
    double getBalance() const { return money; }
    bool couldBuy() const { return availableSlots > 0; }
};