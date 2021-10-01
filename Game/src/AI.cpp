//
// Created by tesserakt on 23.09.2021.
//

#include <AI.h>
#include <range/v3/all.hpp>

GameObject::Id AI::predictToBuy() {
    return ranges::max(world.getSlots(), {}, [&](int slot) { return world.getProfitness(slot); });
}

AI::AI(const World &world, unsigned int maxSlots) : world(world) {
    money = 10000;
    availableSlots = maxSlots;
}

GameObject::Id AI::predictToSell() {
    return ranges::max(getSlots(), {}, [&](int slot) { return getProfitness(slot); });
}

double AI::getProfitness(GameObject::Id itemId) {
    auto item = viewItem(itemId);
    auto sellByKind = ranges::count_if(container, [&](const auto &a) {
        return a.second->name == item.name;
    });
    return (availableSlots + 0.1) / sellByKind / (item.timesSold + 1) * item.cost;
}

