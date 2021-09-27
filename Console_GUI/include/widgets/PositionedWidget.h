#pragma once

#include "SizeableWidget.h"
#include "utils.h"
#include <ncurses.h>

class PositionedWidget : public virtual SizeableWidget {
public:
    virtual void changePos(int x, int y) {
        position.x = x;
        position.y = y;
    }

    virtual void hide(bool hide) { this->isHidden = hide; }

    // light a button
    virtual void turnOn(int color) {
        col = color;
        isBlowing = true;
    };

    // put out the light
    virtual void turnOff() { isBlowing = false; };

protected:
    bool isBlowing;
    int col;        // COLor of Button
    bool isHidden;
    UIPoint position;
    Align align;
};