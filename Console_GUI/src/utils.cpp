#include <algorithm>
#include "utils.h"
#include "iostream"

// Returns height of sentence
unsigned int getHeight(std::string str) {
    return std::count(str.begin(), str.end(), '\n') + 1;
}

// Returns max width of line
// Example: getWidth("one\n"
//                   "three") == 5
unsigned int getWidth(std::string str) {
    int width;
    int maxWidth = 0;
    while ((width = str.find('\n')) != std::string::npos) {
        maxWidth = fmax(width, maxWidth);
        str.erase(0, width + 1);
    }
    if (width == std::string::npos) {
        width = str.length();
        maxWidth = fmax(width, maxWidth);
    }
    return maxWidth;
}

int generateId() {
    static int id = 0;
    return id++;
}

std::string splitBySentence(std::string str, int number) {
    if (number < 0)
        throw std::runtime_error("Wrong number for splitBySentence");

    std::string res;
    int i = 0;
    while (number >= 0) {
        if (i == str.size())
            break;
        if (str[i] == '\n') {
            number--;
            if (number != -1)
                res.clear();
        }
        else
            res.append(1, str[i]);
        i++;
    }
    while (res.size() != getWidth(str)) {
        res.append(1, ' ');
    }
    return res;
}

void quitGame() {
    clear();
    endwin();
    std::cout << "See you later ;)" << std::endl;
    exit(0);
}

