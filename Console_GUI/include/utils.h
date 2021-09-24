#pragma once

#include <cmath>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>

/*
 * This file contains necessary utilities, such as geometric abstractions
 */

int getHeight(std::string str);
int getWidth(std::string str);

template<typename InputIterator>
std::string joinToString(InputIterator begin, InputIterator end, const std::string& separator) {
    auto size = std::distance(begin, end);
    if (size == 0)
        return "";
    std::stringstream ss;
    ss << *begin;
    return std::accumulate(++begin, end, ss.str(), [&separator](const auto& acc, const auto& x) {
        std::stringstream ss;
        ss << x;
        return acc + separator + ss.str();
    });
}


template<typename T>
struct Point {
    const T x, y;

    Point() : x(), y() {}

    Point(T x, T y) : x(x), y(y) {}

    [[nodiscard]] double magnitude() const {
        return x * x + y * y;
    }

    [[nodiscard]] double sqrMagnitude() const {
        return sqrt(magnitude());
    }

    Point<T> operator+(Point<T> rhs) const {
        return Point(x + rhs.x, y + rhs.y);
    }

    Point<T> operator-(Point<T> rhs) const {
        return Point(x - rhs.x, y - rhs.y);
    }
};

using IPoint = Point<int>;
using FPoint = Point<float>;
using UIPoint = Point<unsigned int>;

template<typename T>
struct Vector {
    const Point<T> x, y;

    Vector() : x(), y() {}

    Vector(Point<T> x, Point<T> y) : x(x), y(y) {}

    Vector(T x1, T x2, T y1, T y2) : x(x1, y1), y(x2, y2) {}

    [[nodiscard]] double magnitude() const {
        return (x - y).magnitude();
    }

    [[nodiscard]] double sqrMagnitude() const {
        return (x - y).sqrMagnitude();
    }
};

template<typename T>
struct Rect {
    const Point<T> a, b;

    Rect() : a(), b() {}

    Rect(Point<T> a, Point<T> b) : a(a), b(b) {}

    Rect(T x1, T x2, T y1, T y2) : a(x1, y1), b(x2, y2) {}

    double area() {
        return abs((a.x - b.x) * (a.y - b.y));
    }
};

using IRect = Rect<int>;
using FRect = Rect<float>;
using UIRect = Rect<unsigned int>;

enum Align {
    Left, Right, Centered
};