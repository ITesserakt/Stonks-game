//
// Created by tesserakt on 23.09.2021.
//

#include "Player.h"

Player::Player(unsigned int inventorySize, double money, unsigned int availableSlots, std::map<GameObject::Id, std::unique_ptr<GameObject>> container)
    : inventorySize(inventorySize),
      Gamer(money, availableSlots, std::move(container)) {}

Player::Player() : inventorySize(Config::current().activePreset().inventorySize),
                   Gamer(Config::current().activePreset().initialMoney, Config::current().activePreset().inventorySize, {}) {}

unsigned int Player::getInventorySize() const { return inventorySize; }
