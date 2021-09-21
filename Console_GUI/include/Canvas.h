#pragma once

#include "widgets/Widget.h"

class Canvas : public virtual Widget {
    // TODO: main widget that takes all console window (Menu)

public:
    void show() override {
        for (const auto& child: children) {
            child->show();
        }
    }
};