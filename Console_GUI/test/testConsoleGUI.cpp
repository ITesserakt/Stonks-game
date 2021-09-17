#include "../include/Helper.h"
#include <iostream>

int testAmount = 0;
int mistakes   = 0;

void test(std::string str1, std::string str2) {
    testAmount++;
    if (!str1.compare(str2)) {
        mistakes++;
        std::cout << "Test № " << testAmount << " failed\n"
                  << "We get: " << str1
                  << "\nBut need:" << str2 << '\n';
    }
}

template <typename T>
void test(T a, T b) {
    testAmount++;
    if (a != b) {
        mistakes++;
        std::cout << "Test № " << testAmount << " failed\n"
                  << "We get: " << a
                  << "\nBut need:" << b << '\n';
    }
}

/*This main is for testing GUI module*/
int main() {
    test(getHeight("Hello"), 1);
    test(getHeight("Hello\n\n"),3);
    test(getHeight("\nHel\nlo\n"), 4);

    printf("Results: %d/%d\n", testAmount - mistakes, testAmount);
}