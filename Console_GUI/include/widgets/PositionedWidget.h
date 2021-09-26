#pragma once

#include "Widget.h"
#include "utils.h"
#include <ncurses.h>

class PositionedWidget : public virtual Widget
{
public:
    virtual UISize getSize() = 0;
    bool getBlowing() { return isBlowing; }
    virtual UIPoint getPosition() { return position; }
    virtual void changePos(int x, int y) {
        position.x = x;
        position.y = y;
    }

    // light a button
    virtual void turnOn(int color) { col = color; isBlowing = true; };
    // put out the light
    virtual void turnOff() { isBlowing = false; };

protected:
    bool isBlowing;
    int col;        // COLor of Button

    UISize size;
    UIPoint position;
    Align align;
};