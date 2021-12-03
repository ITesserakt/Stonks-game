#include <fstream>
#include "Config.h"
#include "Debug.h"

void Config::generateConfig() {
    std::ofstream configFile(path, std::fstream::out);
    configFile << R"({
    "Settings": {
        "botsAmount": 3,
        "worldSize": 18,
        "debug": false,
        "debugSpeedGame": 10,
        "maxFPS": 15,
        "difficulty": 0,
        "presets": [{
            "name": "Easy",
            "initialMoney": 1000,
            "winCondition": 10000,
            "inventorySize": 5
        }, {
             "name": "Normal",
             "initialMoney": 200,
             "winCondition": 10000,
             "inventorySize": 3
         }, {
             "name": "Hard",
             "initialMoney": 200,
             "winCondition": 50000,
             "inventorySize": 1
         }]
    }
})";
}

void readConfig(std::map<std::string, jsoncons::json> &settings, const jsoncons::json &json) {
    for (const auto& next: json.object_range()) {
        settings.emplace(next.key(), next.value());
    }
}

Config::Config() {
    std::ifstream configFile(path);
    if (!configFile.good()) {
        generateConfig();
        configFile.open(path);
    }
    jsoncons::json config;
    try {
        config = jsoncons::json::parse(configFile);
    } catch (const std::runtime_error& ex) {
        Debug::logger << ex.what();
        generateConfig();
        throw std::runtime_error("ERROR in config. Config file was reset");
    }
    readConfig(settings, config["Settings"]);
}

const Config &Config::getInstance() {
    static Config instance;
    return instance;
}

#define CONFIG_PROPERTY(t, x) const t Config::x = getInstance().getSettingByName<t>(#x)

CONFIG_PROPERTY(unsigned int, worldSize);
CONFIG_PROPERTY(int, botsAmount);
CONFIG_PROPERTY(int, debugSpeedGame);
CONFIG_PROPERTY(bool, debug);
CONFIG_PROPERTY(int, maxFPS);
CONFIG_PROPERTY(int, difficulty);
CONFIG_PROPERTY(std::vector<DifficultyPreset>, presets);

DifficultyPreset Config::activePreset = presets[difficulty];
