#pragma once

#include "Gamer.h"
#include "Config.h"

class Player : public Gamer {
private:
    unsigned inventorySize;
public:
    explicit Player(unsigned maxSlots = Config::inventorySize) : inventorySize(maxSlots) {
        money = static_cast<double>(Config::initialMoney);
        availableSlots = maxSlots;
    }
    virtual ~Player() = default;
    unsigned int getInventorySize() const { return inventorySize; }
};