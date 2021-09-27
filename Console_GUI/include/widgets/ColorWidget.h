#pragma once

#include "Widget.h"

class ColorWidget: public virtual Widget {
protected:
    bool isBlowing;
    int col;        // COLor of Button
public:
    // light a button
    virtual void turnOn(int color) {
        col = color;
        isBlowing = true;
    };

    // put out the light
    virtual void turnOff() { isBlowing = false; };
};