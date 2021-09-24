//
// Created by tesserakt on 23.09.2021.
//

#include <Gamer.h>

void Gamer::buyItem(std::unique_ptr<GameObject> item) {
    if (item->timesSold != 0 && item->lastSeller != nullptr)
        item->lastSeller->money += item->cost;
    money -= item->cost;
    inventory.emplace_back(std::move(item));
}

std::unique_ptr<GameObject> Gamer::sellItem(GameObject::Id itemId, GameObject::Cost newCost) {
    auto item = takeItem(itemId);
    item->lastSeller = shared_from_this();
    item->cost = newCost;
    item->timesSold++;
    return std::move(item);
}

std::unique_ptr<GameObject> Gamer::takeItem(GameObject::Id itemId) {
    auto item = std::remove_if(inventory.begin(), inventory.end(), [itemId](const auto& x) {
        return x != nullptr && x->id == itemId;
    });
    if (item == inventory.end())
        throw std::runtime_error("Could not take item");
    return std::move(*item);
}

const GameObject &Gamer::viewItem(GameObject::Id itemId) const {
    auto item = std::find_if(inventory.begin(), inventory.end(), [itemId](const std::unique_ptr<GameObject>& x) {
        return x != nullptr && x->id == itemId;
    });
    if (item == inventory.end())
        throw std::runtime_error("Could not find item");
    return **item;
}

