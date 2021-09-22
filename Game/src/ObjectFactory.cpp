#include <algorithm>
#include "ObjectFactory.h"

std::random_device ObjectFactory::engine = std::random_device();

GameObject ObjectFactory::generateNext() {
    auto object = data.begin();
    std::advance(object, random() % data.size() - 1);
    auto name = object->first;
    auto kind = object->second;
    auto descriptions = std::vector<GameObject::Description>();
    for (const auto& [k, _]: kind.descriptions) descriptions.emplace_back(k);
    auto descCount = random() % kind.descriptions.size();

    std::shuffle(descriptions.begin(), descriptions.end(), random);
    auto shuffled = std::vector(descriptions.begin(), descriptions.begin() + descCount + 1);
    auto cost = kind.basicCost *
                std::accumulate(shuffled.begin(), shuffled.end(), 1.0, [&](auto &a, auto &b) { return a * kind.descriptions[b]; });

    return {name, shuffled, cost};
}

ObjectFactory::ObjectFactory(const nlohmann::json &config, unsigned int randomSeed): random(randomSeed) {
    auto objects = config["Objects"];

    for (const auto &object: objects) {
        const auto &name = object["name"].get<GameObject::Name>();
        const auto &cost = object["cost"].get<GameObject::Cost>();
        const auto &descs = object["descriptions"];
        data[name] = ObjectPrototype {{}, cost };
        for (const auto &desc: descs) {
            const auto& d = desc["value"].get<GameObject::Description>();
            const auto& c = desc["multiplier"].get<GameObject::Cost>();
            data[name].descriptions[d] = c;
        }
    }
}