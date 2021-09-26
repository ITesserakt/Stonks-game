#pragma once

#include <utility>

#include "widgets/Widget.h"
#include "widgets/HoverableWidget.h"

enum Direction{ toTheTop, toTheBot};

class Canvas : public virtual Widget {
    // TODO: main widget that takes all console window (Menu)
    // It places all widgets on right place
private:
    Align align;
    void changePos(int x, int y) override {}  // doesn't need to be used

public:
    Canvas(std::string name, Align al) : Widget(std::move(name)), align(al)
    {}
    void show() override;

    std::shared_ptr<HoverableWidget> whoOnHover();
    void changeActiveWidget(Direction direct);

    UISize getSize() override;
};