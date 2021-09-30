#pragma once

#include "Gamer.h"

class Player : public Gamer {
private:
    unsigned availableSlots;
    unsigned inventorySize;
public:
    Player(unsigned maxSlots = 7) :
    availableSlots(maxSlots), inventorySize(maxSlots) {
        money = 5000.;
    }
    virtual ~Player() = default;
    void buyItem(std::unique_ptr<GameObject> item) override;
    unsigned int getInventorySize() { return inventorySize; }
    unsigned int getAvailableSlotsAmount() { return availableSlots; }

    std::unique_ptr<GameObject> sellItem(GameObject::Id itemId, GameObject::Cost newCost) override;

    std::unique_ptr<GameObject> sellItem(std::unique_ptr<GameObject> item,
                                         GameObject::Cost newCost) override;
};