//
// Created by tesserakt on 23.09.2021.
//

#include "Player.h"

void Player::buyItem(std::unique_ptr<GameObject> item) {
    Gamer::buyItem(std::move(item));
}

std::unique_ptr<GameObject> Player::sellItem(GameObject::Id itemId, GameObject::Cost newCost) {
    return Gamer::sellItem(itemId, newCost);
}
