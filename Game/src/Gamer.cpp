//
// Created by tesserakt on 23.09.2021.
//

#include <Gamer.h>

void Gamer::buyItem(std::unique_ptr<GameObject> item) {
    if (item->timesSold != 0 && item->lastSeller != nullptr)
        item->lastSeller->money += item->cost;
    money -= item->cost;
    container[item->id] = std::move(item);
}

// TODO selling item by ID is bad idea, because when you call takeItem, Item is
// is already taken by me. Also it makes useless difference with buyItem.
std::unique_ptr<GameObject> Gamer::sellItem(GameObject::Id itemId, GameObject::Cost newCost) {
    auto item = takeItem(itemId);
    item->lastSeller = shared_from_this();
    item->cost = newCost;
    item->timesSold++;
    return item;
}

std::unique_ptr<GameObject> Gamer::sellItem(std::unique_ptr<GameObject> item, GameObject::Cost newCost) {
    item->lastSeller = shared_from_this();
    item->cost = newCost;
    item->timesSold++;
    return item;
}

std::unique_ptr<GameObject> Gamer::sellItem(GameObject::Id itemId) {
    return sellItem(itemId, viewItem(itemId).cost);
}
