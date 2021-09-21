#include <algorithm>
#include "../include/Helper.h"

int getHeight(std::string str) {
    return std::count(str.begin(), str.end(), '\n') + 1;
}