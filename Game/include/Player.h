#pragma once

#include "Gamer.h"
#include "Config.h"

class Player : public Gamer {
private:
    unsigned int inventorySize;
public:
    explicit Player(unsigned maxSlots = Config::activePreset.inventorySize) : inventorySize(maxSlots) {
        money = Config::activePreset.initialMoney;
        availableSlots = maxSlots;
    }
    virtual ~Player() = default;
    unsigned int getInventorySize() const { return inventorySize; }
};