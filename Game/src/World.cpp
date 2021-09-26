#include <World.h>

double World::getProfitness(const GameObject::Id &itemId) const {
    auto item = viewItem(itemId);
    auto sellByKind = std::count_if(container.begin(), container.end(), [item](const auto &x) {
        return x.second != nullptr && x.second->name == item.name;
    });
    return (1 - factory.getCostForKind(item.name) / item.cost) * (availableSlots + 0.1) / sellByKind /
           (item.timesSold + 1);
}

std::vector<GameObject::Id> World::getSlots() const {
    std::vector<GameObject::Id> res;
    for (const auto &[k, _]: container)
        res.push_back(k);
    return res;
}

World::World(ObjectFactory &&factory, unsigned int maxSlots) : factory(factory), availableSlots(maxSlots) {}

void World::fillUp() {
    while (availableSlots > 0) {
        auto next = factory.generateNext();
        container[next.id] = std::make_unique<GameObject>(next);
        availableSlots--;
    }
}

void World::addPlayer(std::shared_ptr<Gamer> &&gamer) {
    players.emplace_back(gamer);
}

std::unique_ptr<GameObject> World::takeItem(GameObject::Id itemId) {
    auto result = ViewableContainer::takeItem(itemId);
    availableSlots--;
    return result;
}
