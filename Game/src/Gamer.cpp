//
// Created by tesserakt on 23.09.2021.
//

#include <Gamer.h>
#include "Statistics.h"

void Gamer::buyItem(std::unique_ptr<GameObject> item) {
    Stat::Statistic("amountOfPurchases");
    Stat::Statistic("amountOfItemsInWorld", -1);
    if (item->timesSold != 0 && item->lastSeller != nullptr)
        item->lastSeller->money += item->cost;
    money -= item->cost;
    populate(item->id, std::move(item));
    availableSlots--;
}

std::unique_ptr<GameObject> Gamer::sellItem(GameObject::Id itemId, GameObject::Cost newCost) {
    Stat::Statistic("amountOfPurchases");
    auto item = askItem(itemId);
    if (item == nullptr) return item;
    Stat::Statistic("amountOfItemsInWorld", 1);
    Stat::sum(newCost);
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

Gamer::Gamer(double money, unsigned int availableSlots, std::map<GameObject::Id, std::unique_ptr<GameObject>> container)
    : money(money), availableSlots(availableSlots), ViewableContainer(std::move(container)) {}

Gamer::Gamer() : money(0), availableSlots(0) {}

Gamer::Gamer(Gamer &&g) noexcept
    : money(g.money), availableSlots(g.availableSlots.load()), ViewableContainer(std::move(g)) {}

double Gamer::getBalance() const { return money; }

bool Gamer::couldBuy() const { return availableSlots > 0; }
