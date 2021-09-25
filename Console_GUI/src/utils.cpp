#include <algorithm>
#include "utils.h"

int getHeight(std::string str) {
    return std::count(str.begin(), str.end(), '\n') + 1;
}

int getWidth(std::string str) {
    // TODO make it more gibkiy
    return str.length() + 4;
}