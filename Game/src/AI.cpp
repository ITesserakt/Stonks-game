//
// Created by tesserakt on 23.09.2021.
//

#include <AI.h>
#include <range/v3/all.hpp>

GameObject::Id AI::predict() {
    return ranges::max(world.getSlots(), {}, [&](int slot) { return world.getProfitness(slot); });
}

void AI::buyItem(std::unique_ptr<GameObject> item) {
    Gamer::buyItem(std::move(item));
}

std::unique_ptr<GameObject> AI::sellItem(GameObject::Id itemId, GameObject::Cost newCost) {
    return Gamer::sellItem(itemId, newCost);
}

AI::AI(const World &world) : world(world) {}

