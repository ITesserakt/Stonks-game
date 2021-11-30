//
// Created by Сева on 01.12.2021.
//

#include "Statistics.h"

std::map<std::string, int> Stat::Statistic::_callersAndTheirNumber;

int Stat::Statistic::getValueByName(const char *name) {
  if (_callersAndTheirNumber.find(name) != _callersAndTheirNumber.end())
    return _callersAndTheirNumber.at(name);
  else
    return 0;
}
