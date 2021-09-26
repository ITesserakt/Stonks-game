#include <algorithm>
#include "ObjectFactory.h"

std::random_device ObjectFactory::engine = std::random_device();

/*
 * Holy, that's so unoptimized!
 */
GameObject ObjectFactory::generateNext() {
    if (data.empty())
        throw std::runtime_error("Could not generate item from nothing");

    auto object = data.begin();
    std::advance(object, random() % data.size());
    auto name = object->first;
    auto kind = object->second;

    auto descriptions = std::vector<GameObject::Description>();
    for (const auto&[k, _]: kind.descriptions) descriptions.emplace_back(k);
    auto descCount = random() % (kind.descriptions.empty() ? 1 : kind.descriptions.size() + 1);
    std::shuffle(descriptions.begin(), descriptions.end(), random);
    descriptions = std::vector(descriptions.begin(), descriptions.begin() + descCount);

    auto cost = kind.basicCost * std::accumulate(descriptions.begin(), descriptions.end(), 1.0,
                                                 [&](auto &a, auto &b) { return a * kind.descriptions[b]; });

    return {name, descriptions, lastId++, cost};
}

ObjectFactory::ObjectFactory(const nlohmann::json &config, unsigned int randomSeed) : random(randomSeed) {
    auto objects = config["Objects"];

    for (const auto &object: objects) {
        const auto &name = object["name"].get<GameObject::Name>();
        const auto &cost = object["cost"].get<GameObject::Cost>();
        const auto &descs = object["descriptions"];
        data[name] = ObjectPrototype{{}, cost};
        for (const auto &desc: descs) {
            const auto &d = desc["value"].get<GameObject::Description>();
            const auto &c = desc["multiplier"].get<GameObject::Cost>();
            data[name].descriptions[d] = c;
        }
    }
}

GameObject::Cost ObjectFactory::getCostForKind(const GameObject::Name& kind) const {
    return data.at(kind).basicCost;
}
