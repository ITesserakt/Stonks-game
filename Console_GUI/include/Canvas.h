#pragma once

#include <utility>

#include "widgets/Widget.h"

class Canvas : public virtual Widget {
    // TODO: main widget that takes all console window (Menu)
    // It places all widgets on right place
private:
    Align align;
    virtual void changePos(int x, int y) override {}  // doesn't need to be used

public:
    Canvas(std::string name, Align al) : Widget(std::move(name)), align(al) {}
    void show() override;

    Size<unsigned int> getSize() override;
};