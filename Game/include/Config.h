#pragma once

#include <memory>
#include <map>
#include <string>
#include <jsoncons/basic_json.hpp>
#include <jsoncons/json_traits_macros.hpp>

struct DifficultyPreset {
    std::string name;
    double initialMoney;
    int winCondition;
    int inventorySize;
};

JSONCONS_ALL_MEMBER_TRAITS(DifficultyPreset, name, initialMoney, winCondition, inventorySize);

class Config {
private:
    Config();

    static void generateConfig();

    std::map<std::string, jsoncons::json> settings;

public:
    Config(const Config &other) = delete;

    void operator=(const Config &) = delete;

    static const Config &getInstance();

    template<typename T>
    [[nodiscard]] T getSettingByName(const std::string &name) const {
        auto it = settings.find(name);
        if (it == settings.end()) {
            generateConfig();
            throw std::runtime_error("ERROR in config. Config file was reset\n");
        }
        return it->second.as<T>();
    }

    static const unsigned int worldSize;
    static const bool debug;
    static const int botsAmount;
    static const int debugSpeedGame;
    static const int maxFPS;
    static const std::vector<DifficultyPreset> presets;
    static DifficultyPreset activePreset;
};
