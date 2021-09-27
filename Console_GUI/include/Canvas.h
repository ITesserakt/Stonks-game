#pragma once

#include <utility>

#include "widgets/SizeableWidget.h"
#include "widgets/HoverableWidget.h"

enum Direction {
    toTheTop = -1, toTheBot = 1
};

// It places all widgets on right place
class Canvas : public virtual SizeableWidget {
private:
    Align align;
    std::shared_ptr<HoverableWidget> activeWidget;

public:
    Canvas(std::string name, Align al)
            : Widget(std::move(name)), align(al) {}

    void show() override;

    // Makes first hoverable children onHoverStart()
    void firstOnHover();

    void changeActiveWidget(Direction direct);
    auto getActiveWidget() const { return activeWidget; }

    void bind(std::shared_ptr<Widget> widget) override;

    UISize getSize() override;
};