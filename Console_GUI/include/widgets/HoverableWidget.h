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
        isClicable = true;
    }

    // We go out of button
    virtual void onHoverEnd() {
        turnOff();
        isClicable = false;
    }

    // We pushed enter on button and started click
    virtual void click() = 0;

    int getTabIndex() const { return tabIndex; }

    HoverableWidget(int index): tabIndex(index) {}
};

// EventSequence => Frontend
// GuiEvent => change color <== widget, color, action == widget.powerOn(Color)