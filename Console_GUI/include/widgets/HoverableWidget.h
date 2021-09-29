#pragma once

#include "PositionedWidget.h"
#include "ColorWidget.h"

class HoverableWidget : public virtual PositionedWidget, public virtual ColorWidget {
protected:
    std::function<void(HoverableWidget&)> todo;
    int tabIndex;

public:
    // We are on button
    void onHoverStart(){
        turnOn(COLOR_GREEN);

        for (auto x: getChildrenRecursively<ColorWidget>())
            x->turnOn(COLOR_GREEN);
    }

    // We go out of button
    void onHoverEnd() {
        turnOff();

        for (auto x: getChildrenRecursively<ColorWidget>())
            x->turnOff();
    }

    void click() {
        todo(*this);
    }

    void setName(std::string newName) { name = std::move(newName); }

    int getTabIndex() const { return tabIndex; }

    explicit HoverableWidget(int index, std::function<void(
            HoverableWidget&)> f): todo(f), tabIndex(index) {}
};

// EventSequence => Frontend
// GuiEvent => change color <== widget, color, action == widget.powerOn(Color)