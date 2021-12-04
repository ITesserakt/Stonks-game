#pragma once

#include "Gamer.h"
#include "Config.h"

class Player : public Gamer {
private:
    unsigned int inventorySize;

public:
    explicit Player(unsigned maxSlots = Config::current().activePreset().inventorySize) : inventorySize(maxSlots) {
        money = Config::current().activePreset().initialMoney;
        availableSlots = maxSlots;
    }
    virtual ~Player() = default;
    unsigned int getInventorySize() const { return inventorySize; }
};