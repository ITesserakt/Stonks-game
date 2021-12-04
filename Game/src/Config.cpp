#include <fstream>
#include <jsoncons/json.hpp>

#include "Config.h"

void Config::modify(std::function<void(ConfigData &)> &&f) {
    ConfigData copy = current();
    f(copy);
    std::ofstream configFile(path, std::fstream::out);
    jsoncons::encode_json_pretty(copy, configFile);
}

Config::Config() try : data([] {
                           std::ifstream configFile(path);
                           return jsoncons::decode_json<ConfigData>(configFile);
                       }()) {
} catch (...) {
    std::ofstream out(path);
    jsoncons::encode_json_pretty(ConfigData{}, out);
    throw std::runtime_error("Invalid config. Config file was reset");
}

const ConfigData &Config::current() {
    static Config instance;
    return instance.data;
}