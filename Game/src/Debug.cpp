#include "Debug.h"
#include <iomanip>
#include <sstream>

bool Debug::isLogCleared = false;
Debug Debug::logger = {};

Debug::Debug() {
    if (!isLogCleared) {
        std::ofstream(pathToLog,
                      std::ofstream::out | std::ofstream::trunc);
        isLogCleared = true;
    }
}

std::string currentTime() {
    auto now = time(nullptr);
    auto timeStruct = localtime(&now);
    std::stringstream timeStr;
    timeStr << std::setw(2) << std::setfill('0')
            << timeStruct->tm_hour << ":" << timeStruct->tm_min << ":"
            << timeStruct->tm_sec;
    return timeStr.str();
}
