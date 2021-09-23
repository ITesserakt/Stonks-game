#include <World.h>
#include <cmath>

double World::getProfitness(const GameObject::Id &itemId) const {
    auto item = viewItem(itemId);
    auto sellByKind = std::count_if(slots.begin(),  slots.end(), [item](const auto& x) {
        return x != nullptr && x->name == item.name;
    });
    return (1.05 - factory.getCostForKind(item.name) / item.cost) * (availableSlots + 0.1) / sellByKind / (item.timesSold + 1);
}

std::vector<GameObject::Id> World::getSlots() const {
    std::vector<GameObject::Id> res;
    for (const auto &slot: slots)
        if (slot != nullptr)
            res.emplace_back(slot->id);
    return res;
}

World::World(ObjectFactory &&factory, unsigned int maxSlots) : factory(factory), availableSlots(maxSlots) {}

void World::fillUp() {
    while (availableSlots > 0) {
        slots.push_back(std::make_unique<GameObject>(factory.generateNext()));
        availableSlots--;
    }
}

void World::addPlayer(std::shared_ptr<Gamer> &&gamer) {
    players.emplace_back(gamer);
}

std::unique_ptr<GameObject> World::takeItem(GameObject::Id itemId) {
    std::unique_ptr<GameObject> item;
    for (auto &slot: slots)
        if (slot != nullptr && slot->id == itemId) {
            item = std::move(slot);
            slots.remove(item);
            break;
        }

    if (item == nullptr)
        throw std::runtime_error("Could not take item");
    availableSlots++;
    return std::move(item);
}

const GameObject &World::viewItem(GameObject::Id itemId) const {
    GameObject *item;
    for (auto &slot: slots)
        if (slot != nullptr && slot->id == itemId) {
            item = slot.get();
            break;
        }
    if (item == nullptr)
        throw std::runtime_error("Could not view item");
    return *item;
}
