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

    virtual void hide(bool hide = true) {
        this->isHidden = hide;
        for (auto child: getChildrenWithType<PositionedWidget>()) {
            child->hide(isHidden);
        }
    }

    bool isActive() const { return isHidden; }

protected:
    bool isHidden = false;
    UIPoint position;
    Align align;
    UISize size;
};