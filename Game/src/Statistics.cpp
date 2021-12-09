#include "Statistics.h"
#include "Debug.h"
#include <sstream>

auto Stat::ItemStat::_valuesForEveryObject = std::map<GameObject::Id, Stat::ItemStat::sharedList>();
auto Stat::Timer::_nameAndStart = std::map<std::string, time_struct>();

std::map<std::string, int> Stat::Counter::_callersAndTheirNumber;

int Stat::Counter::getValueByName(const char *name) {
    if (_callersAndTheirNumber.find(name) != _callersAndTheirNumber.end())
        return _callersAndTheirNumber[name];
    else
        return 0;
}

Stat::ItemStat::ItemStat(GameObject::Id itemId, double value) {
    if (_valuesForEveryObject.find(itemId) != _valuesForEveryObject.end()) {
        auto lst = _valuesForEveryObject[itemId];
        if (lst->size() + 1 < _historySize)
            lst->push_back(value);
        else {
            lst->pop_front();
            lst->push_back(value);
        }
    } else {
        _valuesForEveryObject.insert(std::pair<GameObject::Id, sharedList>(itemId, std::make_shared<std::list<double>>()));
        _valuesForEveryObject[itemId]->push_back(value);
    }
}
Stat::ItemStat::sharedList Stat::ItemStat::getValuesById(GameObject::Id id) {
    auto it = _valuesForEveryObject.find(id);
    if (it == _valuesForEveryObject.end()) {
        return nullptr;
    }
    return it->second;
}

std::string Stat::Timer::getTimeByName(const std::string &name, const std::string &format) {
    // ctime is used because we don't have << operator for std::chrono::duration
    // and why not

    // buffer stores result string
    char *buffer = new char[format.size() + 1];
    if (_nameAndStart.find(name) == _nameAndStart.end()) {
        Debug::logger << "Timer with name: " << name << " doesn't exist";
    } else {
        _nameAndStart[name].delta += time(nullptr) - _nameAndStart[name].start;
        _nameAndStart[name].start = time(nullptr);
    }

    // creating string with format
    strftime(buffer, format.size() + 1, format.c_str(), localtime(&_nameAndStart[name].delta));
    std::string res = buffer;
    delete[] buffer;
    return res;
}
