#pragma once

#include "Widget.h"
#include "utils.h"
#include <ncurses.h>

class PositionedWidget : public virtual Widget
{
public:
    bool getBlowing() { return isBlowing; }
    virtual UIPoint getPosition() { return position; }
    void changePos(int x, int y) override {
        position.x = x;
        position.y = y;
    }

    void changeName(const std::string &str) override {
        Widget::changeName(str);
        size.width  = getWidth(name);
        size.height = getHeight(name);
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