#pragma once

#include "SizeableWidget.h"
#include "utils.h"
#include <ncurses.h>

class PositionedWidget : public virtual SizeableWidget {
public:
    void changePos(unsigned int x, unsigned int y) {
        position.x = x;
        position.y = y;
    }

    UIPoint getPosition() { return position; }

    virtual void hide(bool hide) { this->isHidden = hide; }

protected:
    bool isHidden;
    UIPoint position;
    Align align;
    UISize size;
};