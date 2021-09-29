#pragma once

#include "Widget.h"

class ColorWidget : public virtual Widget {
protected:
    bool isBlowing = false;
    int col = COLOR_WHITE;        // COLor of Button
public:
    explicit ColorWidget(const std::string &name) : Widget(name) {}

    // light a button
    void turnOn(int color) {
        col = color;
        isBlowing = true;

        for (auto child: getChildrenWithType<ColorWidget>())
            child->turnOn(color);
    };

    // put out the light
    void turnOff() {
        isBlowing = false;

        for (auto child: getChildrenWithType<ColorWidget>())
            child->turnOff();
    };
};