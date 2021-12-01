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

  Statistic(const char * caller, int number = 1) {
      if (_callersAndTheirNumber.find(caller) != _callersAndTheirNumber.end()) {
          _callersAndTheirNumber.at(caller)+=number;
      }
      else {
          _callersAndTheirNumber.insert(std::pair<std::string, int>(caller, number));
      }
  }
};

// Used for counting amount of function calls
#define counter Statistic(__FUNCTION__)
#define sum(value) Statistic(__FUNCTION__, value)
}
