#pragma once

#include "PositionedWidget.h"
#include "../EventSequence.h"

class HoverableWidget : public virtual PositionedWidget {
protected:
    int tabIndex;
public:
    // We are on button
    virtual void onHoverStart(){
        turnOn(COLOR_GREEN);
    }

    // We go out of button
    virtual void onHoverEnd() {
        turnOff();
    }

    // We pushed enter on button and started click
    virtual void click() = 0;

    int getTabIndex() const { return tabIndex; }

    explicit HoverableWidget(int index): tabIndex(index) {}
};

// EventSequence => Frontend
// GuiEvent => change color <== widget, color, action == widget.powerOn(Color)