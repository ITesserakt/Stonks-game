#pragma once

#include <utility>

#include "PositionedWidget.h"

class Group : public virtual PositionedWidget {
public:
    UISize getSize() override {
        unsigned int maxx = 0;
        unsigned int maxy = 0;

        for (auto child : getChildrenWithType<PositionedWidget>()) {
            if (child->getSize().width > maxx) {
                maxx = child->getSize().width;
            }
            if (child->getSize().height > maxy) {
                maxy = child->getSize().height;
            }
        }

        return {maxx, maxy};
    }

    void show() override {
        if (isHidden) return;

        ranges::for_each(children, &Widget::show);
    }

    void hide(bool isHidden) override {
        for (auto child: getChildrenWithType<PositionedWidget>()) {
            child->hide(isHidden);
        }
    }

    explicit Group(std::string name) : Widget(std::move(name)) {}
};