//
// Created by tesserakt on 23.09.2021.
//

#include <Gamer.h>
#include "Statistics.h"

void Gamer::buyItem(std::unique_ptr<GameObject> item) {
    Stat::counter;
    if (item->timesSold != 0 && item->lastSeller != nullptr)
        item->lastSeller->money += item->cost;
    money -= item->cost;
    populate(item->id, std::move(item));
    availableSlots--;
}

std::unique_ptr<GameObject> Gamer::sellItem(GameObject::Id itemId, GameObject::Cost newCost) {
    auto item = askItem(itemId);
    if (item == nullptr) return item;
    item->lastSeller = shared_from_this();
    item->cost = newCost;
    item->timesSold++;
    availableSlots++;
    return item;
}

std::unique_ptr<GameObject> Gamer::sellItem(GameObject::Id itemId) {
    auto item = focusItem(itemId);
    if (!item.has_value()) return {};
    return sellItem(itemId, item->cost);
}
