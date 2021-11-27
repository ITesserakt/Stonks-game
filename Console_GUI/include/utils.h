#pragma once

#include <cmath>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>

// ncurses and lib 'meta' both defines 'meta' entity
#define meta meta_ncurses

#include <ncurses.h>

#undef meta

/*
 * This file contains necessary utilities, such as geometric abstractions
 */

unsigned int getHeight(std::string str);
unsigned int getWidth(std::string str);
std::string splitBySentence(std::string str, int number);

int generateId();

// Size of widget
// width == x; height == y
template<typename T>
struct Size {
    T width, height;
};

using UISize = Size<unsigned int>;
using ISize = Size<int>;
using FSize = Size<float>;

// Simple point that has x and y
template<typename T>
struct Point {
    T x, y;

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