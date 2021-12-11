#pragma once

#include "GameObject.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <jsoncons/json.hpp>
#include <list>
#include <map>
#include <string>
#include <vector>

namespace Stat {
    const std::string pathForSave = "../share/statistic.json";

    class Counter {
    private:
        static std::map<std::string, int> _callersAndTheirNumber;

        JSONCONS_TYPE_TRAITS_FRIEND;

        Counter(std::map<std::string, int> map) {
            _callersAndTheirNumber = map;
        }

    public:
        static int getValueByName(const char *name);

        explicit Counter(const char *caller, int number = 1) {
            if (_callersAndTheirNumber.find(caller) != _callersAndTheirNumber.end()) {
                _callersAndTheirNumber[caller] += number;
            }
            else {
                _callersAndTheirNumber[caller] = number;
            }
        }

        Counter() = default;
    };

    class ItemStat {
    private:
        using sharedList = std::shared_ptr<std::list<double>>;
        static std::map<GameObject::Id, sharedList> _valuesForEveryObject;
        static constexpr unsigned _historySize{40};

        JSONCONS_TYPE_TRAITS_FRIEND;

        ItemStat(std::map<GameObject::Id, sharedList> map) {
            _valuesForEveryObject = map;
        }

    public:
        explicit ItemStat(GameObject::Id itemId, double value);

        ItemStat() = default;

        static sharedList getValuesById(GameObject::Id id);
    };

    class Timer {
    private:
        struct time_struct {
            time_t start;
            time_t delta;
            bool isSave;// do we want to save timer
        };

        static std::map<std::string, time_struct> _nameAndStart;

        JSONCONS_TYPE_TRAITS_FRIEND;

        Timer(std::map<std::string, time_t> map) {
            for (const auto &el : map) {
                _nameAndStart[el.first] = time_struct{time(nullptr), el.second, true};
            }
        }

    public:
        // name of timer
        static void start(const std::string &name, bool restart = false) {
            if (_nameAndStart.find(name) != _nameAndStart.end()) {
                if (restart) {
                    _nameAndStart.at(name).start = time(nullptr);
                    _nameAndStart.at(name).delta = 0;
                }
            }
            else {
                _nameAndStart[name] = time_struct{time(nullptr), 0, true};
            }
        }

        Timer() = default;

        // Check strftime documentation for format usage
        // if something goes wrong with timer check log.txt
        static std::string getTimeByName(const std::string &name, const std::string &format = "%M:%S");

        // Timer with name will not be save
        static void noSave(const std::string &name) {
            if (_nameAndStart.find(name) != _nameAndStart.end())
                _nameAndStart[name].isSave = false;
        }
    };

// Used for counting amount of function calls
#define counter Counter(__FUNCTION__)
#define sum(value) Counter(__FUNCTION__, value)
}// namespace Stat

namespace jsoncons {
    // json for Coutner
    template <>
    struct json_type_traits<json, Stat::Counter> {
        static bool is(const json &j) {
            if (!j.is_object()) return false;
            return true;
        }

        static Stat::Counter as(const json &j) {
            return {j.as<std::map<std::string, int>>()};
        }

        static json to_json(const Stat::Counter &c) {
            return json{c._callersAndTheirNumber};
        }
    };

    // json for ItemStat
    template <>
    struct json_type_traits<json, Stat::ItemStat> {
        static bool is(const json &j) {
            if (!j.is_object()) return false;
            if (j.empty()) return false;
            return true;
        }

        static Stat::ItemStat as(const json &j) {
            if (j.empty())
                throw "blah";
            return {j.as<std::map<GameObject::Id, Stat::ItemStat::sharedList>>()};
        }

        static json to_json(const Stat::ItemStat &c) {
            return json{c._valuesForEveryObject};
        }
    };

    // json for Timer
    template <>
    struct json_type_traits<json, Stat::Timer> {
        static bool is(const json &j) {
            if (!j.is_object()) return false;
            return true;
        }

        static Stat::Timer as(const json &j) {
            return {j.as<std::map<std::string, time_t>>()};
        }

        static json to_json(const Stat::Timer &c) {
            // This map without elements that we don't want to save
            std::map<std::string, time_t> res;
            for (const auto &el : c._nameAndStart) {
                if (el.second.isSave)
                    res[el.first] = el.second.delta;
            }

            return {res};
        }
    };

}// namespace jsoncons
