#pragma once

#include "Gamer.h"

class Player : public Gamer {
private:
    unsigned inventorySize;
public:
    explicit Player(unsigned maxSlots = 7) : inventorySize(maxSlots) {
        money = 5000.;
        availableSlots = maxSlots;
    }
    virtual ~Player() = default;
    unsigned int getInventorySize() const { return inventorySize; }
};