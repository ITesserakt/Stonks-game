#include "Config.h"
#include <fstream>

const std::string configPath = "../share/config.txt";

std::shared_ptr<Configuration> Configuration::sharedConfig = nullptr;

void generateConfig() {
    std::ofstream configFile(configPath, std::fstream::out);
    configFile << "initialMoney 5000\n"
                  "winCondition 10000\n"
                  "botsAmount 3\n"
                  "inventorySize 7\n"
                  "worldSize 18\n"
                  "debug 0";
}

void readConfig(std::map<std::string, int>& settings, std::ifstream& configFile) {
    int value;
    std::string param;
    while (configFile >> param >> value) {
        settings.emplace(param, value);
    }
}

Configuration::Configuration() {
    std::ifstream configFile(configPath);
    if (!configFile.good()) {
        generateConfig();
        configFile.open(configPath);
    }
    readConfig(settings, configFile);
}

std::shared_ptr<Configuration> Configuration::getInstance() {
    if (sharedConfig == nullptr) {
        sharedConfig = std::shared_ptr<Configuration>(new Configuration);
    }
    return sharedConfig;
}

auto Configuration::getSettingByName(const std::string &name) -> decltype(settings.cbegin()->second){
    auto it = settings.find(name);
    if (it == settings.end()) {
        generateConfig();
        throw std::runtime_error("ERROR in config. Config file was reset\n");
    }
    return it->second;
}

void Configuration::refresh() {
    generateConfig();
    std::ifstream configFile(configPath);
    readConfig(settings, configFile);
}
