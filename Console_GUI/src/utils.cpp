#include <algorithm>
#include "utils.h"

// Returns height of sentence
int getHeight(std::string str) {
    return std::count(str.begin(), str.end(), '\n') + 1;
}

// Returns max width of line
// Example: getWidth("one\n"
//                   "three") == 5
int getWidth(std::string str) {
    int width;
    int maxWidth = 0;
    while ((width = str.find('\n')) != std::string::npos) {
        maxWidth = fmax(width, maxWidth);
        str.erase(0, width);
    }
    if (width == std::string::npos) {
        width = str.length();
        maxWidth = fmax(width, maxWidth);
    }
    return maxWidth;
}