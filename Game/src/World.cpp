#include <World.h>
#include <cmath>

double World::getProfitness(const GameObject::Id &itemId) {
    auto item = std::find_if(slots.begin(), slots.end(), [itemId](const std::unique_ptr<GameObject>& x) {
        return x->id == itemId;
    });
    if (item == slots.end())
        throw std::runtime_error("Gameobject not found");
    auto sellByKind = std::count_if(slots.begin(),  slots.end(), [item](const auto& x) {
        return x->name == (*item)->name;
    });
    return (1.05 - factory.getCostForKind((*item)->name) / (*item)->cost) * (availableSlots + 0.1) / sellByKind / ((*item)->timesSold + 1);
}

std::vector<GameObject::Id> World::getSlots() {
    std::vector<GameObject::Id> res(slots.size());
    std::transform(slots.begin(), slots.end(), res.begin(), [](const auto& x) { return x->id; });
    return res;
}

World::World(ObjectFactory &&factory) : factory(factory) {}
