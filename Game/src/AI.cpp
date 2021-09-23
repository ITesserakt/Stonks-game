//
// Created by tesserakt on 23.09.2021.
//

#include <AI.h>

GameObject::Id AI::predict() {
    return *std::max_element(world.getSlots().begin(), world.getSlots().end(), [this](GameObject::Id a, GameObject::Id b) {
       return world.getProfitness(a) > world.getProfitness(b);
    });
}

