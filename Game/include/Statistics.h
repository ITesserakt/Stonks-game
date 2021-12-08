#pragma once

#include "GameObject.h"
#include <iostream>
#include <jsoncons/json.hpp>
#include <list>
#include <map>
#include <string>
#include <fstream>

namespace Stat {
    const std::string pathForSave = "../share/statistic.json";

    class Counter {
    private:
        static std::map<std::string, int> _callersAndTheirNumber;

        JSONCONS_TYPE_TRAITS_FRIEND;

        Counter(std::map<std::string, int> mapa) {
            _callersAndTheirNumber = mapa;
        }
    public:
        static int getValueByName(const char *name);

        explicit Counter(const char *caller, int number = 1) {
            if (_callersAndTheirNumber.find(caller) != _callersAndTheirNumber.end()) {
                _callersAndTheirNumber.at(caller) += number;
            } else {
                _callersAndTheirNumber.insert(std::pair<std::string, int>(caller, number));
            }
        }

        Counter() = default;

        //~Counter() {
        //    std::ofstream out(pathForSave);
        //    jsoncons::encode_json_pretty(*this, out);
        //}
    };

    class ItemStat {
    private:
        using sharedList = std::shared_ptr<std::list<double>>;
        static std::map<GameObject::Id, sharedList> _valuesForEveryObject;
        static constexpr unsigned _historySize{40};

        JSONCONS_TYPE_TRAITS_FRIEND;

        ItemStat(std::map<GameObject::Id, sharedList> values) {
            _valuesForEveryObject = values;
        }

    public:
        explicit ItemStat(GameObject::Id itemId, double value);

        ItemStat() = default;

        static sharedList getValuesById(GameObject::Id id);
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

        static json to_json(const Stat::Counter& c) {
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

        static json to_json(const Stat::ItemStat& c) {
            return json{c._valuesForEveryObject};
        }
    };

}// namespace jsoncons
