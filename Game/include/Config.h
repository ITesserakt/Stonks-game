#pragma once

#include <jsoncons/basic_json.hpp>
#include <jsoncons/json_traits_macros.hpp>
#include <map>
#include <memory>
#include <string>

struct DifficultyPreset {
    std::string name;
    double initialMoney;
    int winCondition;
    int inventorySize;
};

struct ConfigData {
    unsigned int botsAmount = 3;
    unsigned int worldSize = 18;
    bool debug = false;
    unsigned int debugSpeedGame = 10;
    unsigned int maxFPS = 15;
    unsigned int difficulty = 0;
    std::vector<DifficultyPreset> presets = {
            {"Easy", 1000, 10000, 5},
            {"Normal", 200, 10000, 3},
            {"Hard", 200, 50000, 1}};

    [[nodiscard]] const DifficultyPreset &activePreset() const { return presets[difficulty]; }
};

JSONCONS_ALL_MEMBER_TRAITS(ConfigData, botsAmount, worldSize, debug, debugSpeedGame, maxFPS, difficulty, presets)
JSONCONS_ALL_MEMBER_TRAITS(DifficultyPreset, name, initialMoney, winCondition, inventorySize);

class Config {
private:
    Config();

    const ConfigData data;

public:
    Config(const Config &other) = delete;

    void operator=(const Config &) = delete;

    constexpr static auto path = "../share/config.json";

    static const ConfigData &current();

    static void modify(std::function<void(ConfigData &)> &&f = [](ConfigData &) {});
};
