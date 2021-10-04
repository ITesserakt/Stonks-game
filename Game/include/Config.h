#pragma once

#include <memory>
#include <map>
#include <string>

class Config {
private:
    static Config* sharedConfig;
    Config();

    std::map<std::string, int> settings;

public:
    Config(const Config& other) = delete;
    void operator =(const Config& ) = delete;

    static const Config& getInstance();
    [[nodiscard]] auto getSettingByName(const std::string& name) const -> decltype(settings.cbegin()->second);
    static void refresh();

    static const int inventorySize;
    static const int initialMoney;
    static const int winCondition;
    static const int worldSize;
    static const bool debug;
    static const int botsAmount;
};
