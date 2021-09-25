#pragma once

#include "widgets/Widget.h"

class Canvas : public virtual Widget {
    // TODO: main widget that takes all console window (Menu)
    // It places all widgets on right place
private:
    Align align;

public:
    Canvas(std::string name, Align al) : Widget(name), align(al) {}
    void show() override;

    Size<unsigned int> getSize() override;
};