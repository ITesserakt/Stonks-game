#include <range/v3/all.hpp>

#include "World.h"
#include "Statistics.h"

double World::getProfitness(const GameObject::Id &itemId) const {
    auto item = focusItem(itemId);
    if (!item.has_value()) return std::numeric_limits<double>::quiet_NaN();

    unsigned int sellByKind = 0;
    for (const auto &pair : view())
        if (pair.second.name == item->name)
            sellByKind++;

    return (1 - item->cost / factory.getCostForKind(item->name)) *
           (availableSlots + 0.1) / sellByKind /
           (item->timesSold + 1);
}

World::World(ObjectFactory &&factory, unsigned int maxSlots) : availableSlots(maxSlots),
                                                               factory(factory) {}

void World::fillUp() {
    while (availableSlots > 0) {
        Stat::Statistic("amountOfItemsInWorld", 1);
        auto next = factory.generateNext();
        populate(next.id, std::make_unique<GameObject>(next));
        availableSlots--;
    }
}

void World::addGamer(std::shared_ptr<Gamer> &&gamer) {
    players.emplace_back(gamer);
}

std::unique_ptr<GameObject> World::takeItem(GameObject::Id itemId) {
    auto result = ViewableContainer::askItem(itemId);
    if (result != nullptr)
        availableSlots++;
    return result;
}

void World::putItem(std::unique_ptr<GameObject> obj) {
    populate(obj->id, std::move(obj));
}
