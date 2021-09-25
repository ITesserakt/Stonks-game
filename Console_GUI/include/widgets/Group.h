#pragma once

#include <utility>

#include "PositionedWidget.h"

class Group : public virtual PositionedWidget {
protected:
    std::vector<std::shared_ptr<PositionedWidget>> children;

public:
    Size<unsigned> getSize() override {
        unsigned int minx = -1, miny = -1, maxx = 0, maxy = 0;

        for (const auto &child: children) {
            if (child->getSize().width < minx) {
                minx = child->getSize().width;
            }
            if (child->getSize().width > maxx) {
                maxx = child->getSize().width;
            }
            if (child->getSize().height < miny) {
                miny = child->getSize().height;
            }
            if (child->getSize().height > maxy) {
                maxy = child->getSize().height;
            }
        }

        return {maxx, maxy};
    }

    void show() override {
        for (const auto& child: children) {
            child->show();
        }
    }

    explicit Group(std::string name) : Widget(std::move(name)) {}
};