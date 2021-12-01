#pragma once

#include <utility>

#include "widgets/BindableWidget.h"
#include "widgets/HoverableWidget.h"
#include "widgets/SizeableWidget.h"

enum Direction {
    UP = -1,
    DOWN = 1
};

// It places all widgets on right place
class Canvas : public SizeableWidget, public BindableWidget {
private:
    Align align;
    std::shared_ptr<HoverableWidget> activeWidget;

public:
    Canvas(const std::string &name, Align al)
        : Widget(name),
          align(al) {}

    void show() override;

    void changeActiveWidget(Direction direct, unsigned int length = 1);
    auto getActiveWidget() const { return activeWidget; }

    void bind(std::shared_ptr<Widget> widget) override;

    UISize getSize() override;
};