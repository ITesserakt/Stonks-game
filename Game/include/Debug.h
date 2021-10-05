#pragma once

#include <fstream>
#include <string>
#include <thread>

class Debug {
    const std::string pathToLog = "../share/log.txt";
    static bool isLogCleared;
    Debug();
public:
    Debug(const Debug&) = delete;
    Debug& operator=(const Debug&) = delete;
    static Debug logger;
    template <typename T>
    friend Debug& operator <<(Debug& a, T b);
};

std::string currentTime();

template <typename T>
Debug& operator <<(Debug& a, T b) {
    std::ofstream(a.pathToLog, std::fstream::app) << "ThreadId: "
    << std::this_thread::get_id() << ' ' << currentTime()
    << ' ' << b << std::endl;
    return a;
}

