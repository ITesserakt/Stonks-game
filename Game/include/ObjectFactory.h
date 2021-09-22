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

    static std::random_device engine;
    std::mt19937 random;

public:
    GameObject generateNext();

    explicit ObjectFactory(const nlohmann::json &config, unsigned int randomSeed = engine());

    static ObjectFactory fromFile(const std::string &filename) {
        return ObjectFactory(nlohmann::json::parse(filename));
    }

    static ObjectFactory fromText(const std::string &text) { return ObjectFactory(nlohmann::json::parse(text.begin(),  text.end())); }
};