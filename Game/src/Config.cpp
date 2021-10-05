#include "Config.h"
#include <fstream>
#include <nlohmann/json.hpp>

constexpr auto configPath = "../share/config.json";

Config *Config::sharedConfig = nullptr;

void Config::generateConfig() {
    std::ofstream configFile(configPath, std::fstream::out);
    configFile << R"({
    "Settings": {
        "initialMoney": 200,
        "winCondition": 10000,
        "botsAmount": 3,
        "inventorySize": 5,
        "worldSize": 18,
        "debug": false,
        "debugSpeedGame": 10
    }
})";
}

void readConfig(std::map<std::string, std::string> &settings, const nlohmann::json &json) {
    for (const auto&[k, v]: json.items()) {
        settings.emplace(k, v.dump());
    }
}

Config::Config() {
    std::ifstream configFile(configPath);
    if (!configFile.good()) {
        generateConfig();
        configFile.open(configPath);
    }
    nlohmann::json config;
    try {
        config = nlohmann::json::parse(configFile);
    } catch (...) {
        generateConfig();
        throw std::runtime_error("ERROR in config. Config file was reset");
    }
    readConfig(settings, config["Settings"]);
}

const Config &Config::getInstance() {
    if (sharedConfig == nullptr) {
        sharedConfig = new Config();
    }
    return *sharedConfig;
}

void Config::refresh() {
    generateConfig();
    sharedConfig = new Config();
}

#define CONFIG_PROPERTY(t, x) const t Config::x = getInstance().getSettingByName<t>(#x)

CONFIG_PROPERTY(int, inventorySize);
CONFIG_PROPERTY(int, initialMoney);
CONFIG_PROPERTY(int, winCondition);
CONFIG_PROPERTY(int, worldSize);
CONFIG_PROPERTY(int, botsAmount);
CONFIG_PROPERTY(int, debugSpeedGame);
CONFIG_PROPERTY(bool, debug);
