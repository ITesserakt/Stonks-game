#pragma once

#include "Widget.h"
#include "utils.h"
#include <ncurses.h>

class PositionedWidget : public virtual Widget
{
public:
    virtual Size<unsigned> getSize() = 0;
    virtual UIPoint getPosition() { return position; }
    virtual void changePos(int x, int y) {
        position.x = x;
        position.y = y;
    }

protected:
    Size<unsigned> size;
    UIPoint position;
    Align align;
};