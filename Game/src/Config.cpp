#include "Config.h"
#include <fstream>

const std::string configPath = "../share/config.txt";

Config *Config::sharedConfig = nullptr;

void generateConfig() {
    std::ofstream configFile(configPath, std::fstream::out);
    configFile << "initialMoney 5000\n"
                  "winCondition 10000\n"
                  "botsAmount 3\n"
                  "inventorySize 7\n"
                  "worldSize 18\n"
                  "debug 0";
}

void readConfig(std::map<std::string, int> &settings, std::ifstream &configFile) {
    int value;
    std::string param;
    while (configFile >> param >> value) {
        settings.emplace(param, value);
    }
}

Config::Config() {
    std::ifstream configFile(configPath);
    if (!configFile.good()) {
        generateConfig();
        configFile.open(configPath);
    }
    readConfig(settings, configFile);
}

const Config &Config::getInstance() {
    if (sharedConfig == nullptr) {
        sharedConfig = new Config();
    }
    return *sharedConfig;
}

auto Config::getSettingByName(const std::string &name) const -> decltype(settings.cbegin()->second) {
    auto it = settings.find(name);
    if (it == settings.end()) {
        generateConfig();
        throw std::runtime_error("ERROR in config. Config file was reset\n");
    }
    return it->second;
}

void Config::refresh() {
    generateConfig();
    sharedConfig = new Config();
}

#define CONFIG_PROPERTY(x) const int Config::x = getInstance().getSettingByName(#x)

CONFIG_PROPERTY(inventorySize);
CONFIG_PROPERTY(initialMoney);
CONFIG_PROPERTY(winCondition);
CONFIG_PROPERTY(worldSize);
CONFIG_PROPERTY(botsAmount);

const bool Config::debug = bool(getInstance().getSettingByName("debug"));
