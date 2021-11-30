#pragma once

#include <iostream>
#include <string>
#include <map>

namespace Stat {

class Statistic {
private:
  static std::map<std::string, int> _callersAndTheirNumber;

public:
  static int getValueByName(const char* name);

  Statistic(const char * caller) {
    if (_callersAndTheirNumber.find(caller) != _callersAndTheirNumber.end()) {
      _callersAndTheirNumber.at(caller)++;
    }
    else {
      _callersAndTheirNumber.insert(std::pair<std::string, int>(caller, 1));
    }
  }
};

#define counter Statistic(__FUNCTION__)
}
