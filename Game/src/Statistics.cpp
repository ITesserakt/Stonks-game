#include "Statistics.h"
#include <sstream>

auto Stat::ItemStat::_valuesForEveryObject = std::map<GameObject::Id, Stat::ItemStat::sharedList>();

std::map<std::string, int> Stat::Counter::_callersAndTheirNumber;

int Stat::Counter::getValueByName(const char *name) {
  if (_callersAndTheirNumber.find(name) != _callersAndTheirNumber.end())
    return _callersAndTheirNumber.at(name);
  else
    return 0;
}
Stat::ItemStat::ItemStat(GameObject::Id itemId, double value) {
    if (_valuesForEveryObject.find(itemId) != _valuesForEveryObject.end()) {
        auto lst = _valuesForEveryObject.at(itemId);
        if (lst->size() + 1 < _historySize)
            lst->push_back(value);
        else {
            lst->pop_front();
            lst->push_back(value);
        }
    } else {
        _valuesForEveryObject.insert(std::pair<GameObject::Id, sharedList>(itemId, std::make_shared<std::list<double>>()));
        _valuesForEveryObject.at(itemId)->push_back(value);
    }
}
Stat::ItemStat::sharedList Stat::ItemStat::getValuesById(GameObject::Id id) {
    auto it = _valuesForEveryObject.find(id);
    if (it == _valuesForEveryObject.end()) {
        return nullptr;
    }
    return it->second;
}
