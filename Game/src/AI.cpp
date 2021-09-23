//
// Created by tesserakt on 23.09.2021.
//

#include <AI.h>

GameObject::Id AI::predict() {
    auto slots = world.getSlots();
    GameObject::Cost max = -std::numeric_limits<GameObject::Cost>::infinity();
    GameObject::Id maxId;
    for (auto slot: slots) {
        GameObject::Cost next = world.getProfitness(slot);
        if (next > max) {
            max = next;
            maxId = slot;
        }
    }
    return maxId;
}

void AI::buyItem(std::unique_ptr<GameObject> item) {
    Gamer::buyItem(std::move(item));
}

std::unique_ptr<GameObject> AI::sellItem(GameObject::Id itemId, GameObject::Cost newCost) {
    return Gamer::sellItem(itemId, newCost);
}

AI::AI(const World &world) : world(world) {}

