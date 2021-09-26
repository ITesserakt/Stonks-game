#pragma once

#include <utility>

#include "widgets/Widget.h"
#include "widgets/HoverableWidget.h"

enum Direction {
    toTheTop, toTheBot
};

// It places all widgets on right place
class Canvas : public virtual Widget {
private:
    Align align;

    void changePos(int x, int y) override {}  // doesn't need to be used

public:
    Canvas(std::string name, Align al)
            : Widget(std::move(name)), align(al) {}

    void show() override;

    std::shared_ptr<HoverableWidget> whoOnHover();

    // Makes first hoverable children onHoverStart()
    void firstOnHover();

    void changeActiveWidget(Direction direct);

    UISize getSize() override;
};