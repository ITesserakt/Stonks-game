//
// Created by tesserakt on 23.09.2021.
//

#include "Statistics.h"
#include <Gamer.h>

unsigned int Gamer::IdGenerator = 0U;

void Gamer::buyItem(std::unique_ptr<GameObject> item) {
    // Collecting statistic
    Stat::Timer::start("purchaseTimer", true);
    Stat::Counter("amountOfPurchases");
    Stat::Counter("amountOfItemsInWorld", -1);
    // End of statistic

    if (item->timesSold != 0 && item->lastSeller != nullptr) item->lastSeller->money += item->cost;
    money -= item->cost;
    populate(item->id, std::move(item));
    availableSlots--;
}

std::unique_ptr<GameObject> Gamer::sellItem(GameObject::Id itemId, GameObject::Cost newCost) {
    auto item = askItem(itemId);
    if (item == nullptr) return item;
    item->lastSeller = shared_from_this();
    item->cost       = newCost;

    // Collecting statistic
    Stat::Timer::start("purchaseTimer", true);
    Stat::Counter("amountOfPurchases");
    Stat::ItemStat(itemId, newCost);
    Stat::Counter("amountOfItemsInWorld", 1);
    Stat::sum(newCost);
    // End of statistic

    item->timesSold++;
    availableSlots++;
    return item;
}

std::unique_ptr<GameObject> Gamer::sellItem(GameObject::Id itemId) {
    auto item = focusItem(itemId);
    if (!item.has_value()) return {};
    return sellItem(itemId, item->cost);
}

Gamer::Gamer(double                                           money,
        unsigned int                                          availableSlots,
        unsigned int                                          gamerId,
        std::map<GameObject::Id, std::unique_ptr<GameObject>> container)
    : money(money), availableSlots(availableSlots), ViewableContainer(std::move(container)), id(gamerId) {}

Gamer::Gamer() : money(0), availableSlots(0), id(IdGenerator++) {}

Gamer::Gamer(Gamer &&g) noexcept
    : money(g.money), availableSlots(g.availableSlots.load()), ViewableContainer(std::move(g)), id(IdGenerator++) {}

double Gamer::getBalance() const {
    return money;
}

unsigned int Gamer::getId() const {
    return id;
}

bool Gamer::couldBuy() const {
    return availableSlots > 0;
}

void Gamer::takeTaxes(TaxStrategy &strategy) {
    money = money - strategy.calcTax(money);
}
