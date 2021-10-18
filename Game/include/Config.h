#pragma once

#include <memory>
#include <map>
#include <string>
#include <jsoncons/basic_json.hpp>

class Config {
private:
    static Config *sharedConfig;

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

    static void refresh();

    static const int inventorySize;
    static const int initialMoney;
    static const int winCondition;
    static const int worldSize;
    static const bool debug;
    static const int botsAmount;
    static const int debugSpeedGame;
};
