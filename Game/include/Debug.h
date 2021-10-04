#pragma once

class Debug {
    const string pathToLog = "../share/log.txt";
public:
    template <typename T>
    friend Debug& operator <<(Debug& a, T b);
};

template <typename T>
Debug& operator <<(Debug& a, T b) {
    fstream(a.pathToLog, fstream::app) << b;
    return a;
}
