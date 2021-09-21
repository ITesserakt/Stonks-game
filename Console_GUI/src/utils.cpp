#include <algorithm>
#include "utils.h"

int getHeight(std::string str) {
    return std::count(str.begin(), str.end(), '\n') + 1;
}