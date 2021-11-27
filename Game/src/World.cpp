#include <range/v3/all.hpp>

#include "World.h"

double World::getProfitness(const GameObject::Id &itemId) const {
    auto item = viewItem(itemId);
    auto sellByKind = ranges::count_if(container, [&](const auto &a) {
        return a.second->name == item.name;
    });
    return (1 - item.cost / factory.getCostForKind(item.name)) *
           (availableSlots + 0.1) / sellByKind /
           (item.timesSold + 1);
}

World::World(ObjectFactory &&factory, unsigned int maxSlots) :
        availableSlots(maxSlots),
        factory(factory) {}

void World::fillUp() {
    while (availableSlots > 0) {
        auto next = factory.generateNext();
        container[next.id] = std::make_unique<GameObject>(next);
        availableSlots--;
    }
}

void World::addGamer(std::shared_ptr<Gamer> &&gamer) {
    players.emplace_back(gamer);
}

std::unique_ptr<GameObject> World::takeItem(GameObject::Id itemId) {
    auto result = ViewableContainer::takeItem(itemId);
    availableSlots++;
    return result;
}
