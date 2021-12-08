#pragma once

#include "GameObject.h"
#include <iostream>
#include <list>
#include <map>
#include <string>

namespace Stat {

    class Counter {
    private:
        static std::map<std::string, int> _callersAndTheirNumber;

    public:
        static int getValueByName(const char *name);

        explicit Counter(const char *caller, int number = 1) {
            if (_callersAndTheirNumber.find(caller) != _callersAndTheirNumber.end()) {
                _callersAndTheirNumber.at(caller) += number;
            } else {
                _callersAndTheirNumber.insert(std::pair<std::string, int>(caller, number));
            }
        }
    };

    class ItemStat {
    private:
        using sharedList = std::shared_ptr<std::list<double>>;
        static std::map<GameObject::Id, sharedList> _valuesForEveryObject;
        static constexpr unsigned _historySize{40};

    public:
        explicit ItemStat(GameObject::Id itemId, double value);

        static sharedList getValuesById(GameObject::Id id);
    };

// Used for counting amount of function calls
#define counter Counter(__FUNCTION__)
#define sum(value) Counter(__FUNCTION__, value)
}// namespace Stat
