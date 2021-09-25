#pragma once

#include <utility>

#include "PositionedWidget.h"

class Group : public virtual PositionedWidget {
public:
    Size<unsigned> getSize() override {
        unsigned int maxx = 0;
        unsigned int maxy = 0;

        for (const auto &child: children) {
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
        for (const auto& child: children) {
            child->show();
        }
    }

    explicit Group(std::string name) : Widget(std::move(name)) {}
};