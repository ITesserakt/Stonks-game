#pragma once

#include "Widget.h"

class ColorWidget : public virtual Widget {
protected:
    bool isBlowing = false;
    int col = COLOR_WHITE;        // COLor of Button
public:
    explicit ColorWidget(const std::string &name);

    // light a button
    void turnOn(int color);

    // put out the light
    void turnOff();
};