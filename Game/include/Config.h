#pragma once

#include <memory>
#include <map>
#include <string>

class Configuration {
private:
    static std::shared_ptr<Configuration> sharedConfig;
    Configuration();

    std::map<std::string, int> settings;

public:
    Configuration(Configuration& other) = delete;
    void operator =(const Configuration& ) = delete;

    static std::shared_ptr<Configuration> getInstance();
    auto getSettingByName(const std::string& name) -> decltype(settings.cbegin()->second);
    void refresh();
};
