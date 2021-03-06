#include "ObjectFactory.h"
#include "Statistics.h"
#include <range/v3/all.hpp>

std::random_device ObjectFactory::engine = {};

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

    auto descriptions = kind.descriptions | ranges::views::keys |
                        ranges::to<std::vector<std::string>> |
                        ranges::actions::shuffle(random);
    auto descCount = random() %
                     (kind.descriptions.empty() ? 1 : kind.descriptions.size() + 1);
    descriptions = std::vector<GameObject::Description>(descriptions.begin(),
                                                        descriptions.begin() + descCount);

    auto cost = kind.basicCost * ranges::accumulate(descriptions, 1.0,
                                                    [&](const auto &acc,
                                                        const auto &b) {
                                                        return acc *
                                                               kind.descriptions[b];
                                                    });

    // Collecting statistic
    Stat::ItemStat(lastId, cost);

    return {name, descriptions, lastId++, cost};
}

ObjectFactory::ObjectFactory(const jsoncons::json &config,
                             unsigned int randomSeed) : random(randomSeed) {
    auto objects = config["Objects"];

    for (const auto &object : objects.array_range()) {
        const auto &name = object["name"].as<GameObject::Name>();
        const auto &cost = object["cost"].as<GameObject::Cost>();
        const auto &descs = object["descriptions"];
        data[name] = ObjectPrototype{{}, cost};
        for (const auto &member : descs.object_range()) {
            data[name].descriptions[member.key()] = member.value().as<GameObject::Cost>();
        }
    }
}

GameObject::Cost
ObjectFactory::getCostForKind(const GameObject::Name &kind) const {
    return data.at(kind).basicCost;
}

ObjectFactory ObjectFactory::fromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open specified file");
    return ObjectFactory(jsoncons::json::parse(file));
}

ObjectFactory ObjectFactory::fromText(const std::string &text) {
    return ObjectFactory(jsoncons::json::parse(text.begin(), text.end()));
}

ObjectFactory ObjectFactory::empty() {
    return ObjectFactory::fromText(R"({"Objects":[]})");
}
