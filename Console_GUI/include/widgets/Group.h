#pragma once

#include <utility>

#include "PositionedWidget.h"

class Group : public virtual PositionedWidget {
protected:
    std::vector<std::shared_ptr<PositionedWidget>> children;

public:
    UIRect getSize() override {
        unsigned int minx = -1, miny = -1, maxx = 0, maxy = 0;

        for (const auto &child: children) {
            if (child->getSize().a.x < minx) {
                minx = child->getSize().a.x;
            }
            if (child->getSize().b.x > maxx) {
                maxx = child->getSize().a.x;
            }
            if (child->getSize().a.y < miny) {
                miny = child->getSize().a.y;
            }
            if (child->getSize().b.x > maxy) {
                maxy = child->getSize().a.y;
            }
        }

        if (minx == -1 || miny == -1) {
            minx = 0;
            miny = 0;
        }

        return {minx, miny, maxx, maxy};
    }

    void show() override {
        for (const auto& child: children) {
            child->show();
        }
    }

    explicit Group(std::string name) : Widget(std::move(name)) {}
};