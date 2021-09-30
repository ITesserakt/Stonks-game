//
// Created by tesserakt on 23.09.2021.
//

#include "Player.h"

void Player::buyItem(std::unique_ptr<GameObject> item) {
    if (availableSlots > 0) {
        Gamer::buyItem(std::move(item));
        availableSlots--;
    }
}

std::unique_ptr<GameObject> Player::sellItem(GameObject::Id itemId, GameObject::Cost newCost) {
    availableSlots++;
    return Gamer::sellItem(itemId, newCost);
}

std::unique_ptr<GameObject>
Player::sellItem(std::unique_ptr<GameObject> item, GameObject::Cost newCost) {
    availableSlots++;
    return Gamer::sellItem(std::move(item), newCost);
}
