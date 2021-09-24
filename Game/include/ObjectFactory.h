#pragma once

#include <vector>
#include <string>
#include <map>
#include <random>
#include <fstream>
#include <nlohmann/json.hpp>
#include "GameObject.h"

class ObjectFactory {
    struct ObjectPrototype {
        std::map<GameObject::Description, GameObject::Cost> descriptions;
        GameObject::Cost basicCost;
    };

private:
    std::map<GameObject::Name, ObjectPrototype> data;
    unsigned int lastId = 0;

    static std::random_device engine;
    std::mt19937 random;

public:
    GameObject generateNext();
    GameObject::Cost getCostForKind(const GameObject::Name& kind) const;

    explicit ObjectFactory(const nlohmann::json &config, unsigned int randomSeed = engine());

    static ObjectFactory fromFile(const std::string &filename) {
        return ObjectFactory(nlohmann::json::parse(filename));
    }

    static ObjectFactory fromText(const std::string &text) { return ObjectFactory(nlohmann::json::parse(text.begin(),  text.end())); }

    static ObjectFactory empty() { return ObjectFactory(R"({"Objects":[]})"); }
};