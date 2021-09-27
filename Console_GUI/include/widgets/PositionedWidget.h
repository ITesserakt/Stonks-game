#pragma once

#include "SizeableWidget.h"
#include "utils.h"
#include <ncurses.h>

class PositionedWidget : public virtual SizeableWidget {
public:
    void changePos(int x, int y) {
        position.x = x;
        position.y = y;
    }

    void changeName(const std::string &str) override {
        Widget::changeName(str);
        size.width  = getWidth(name);
        size.height = getHeight(name);
    }

    virtual void hide(bool hide) { this->isHidden = hide; }

protected:
    bool isHidden;
    UIPoint position;
    Align align;
    UISize size;
};