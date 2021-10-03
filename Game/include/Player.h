#pragma once

#include "Gamer.h"
#include "Config.h"

class Player : public Gamer {
private:
    unsigned inventorySize;
public:
    explicit Player(unsigned maxSlots = Configuration::getInstance()->getSettingByName("inventorySize")) : inventorySize(maxSlots) {
        money = static_cast<double>(Configuration::getInstance()->getSettingByName("initialMoney"));
        availableSlots = maxSlots;
    }
    virtual ~Player() = default;
    unsigned int getInventorySize() const { return inventorySize; }
};